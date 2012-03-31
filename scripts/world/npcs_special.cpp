/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Npcs_Special
SD%Complete: 100
SDComment: To be used for special NPCs that are located globally.
SDCategory: NPCs
EndScriptData
*/

#include "precompiled.h"
#include "escort_ai.h"
#include "ObjectMgr.h"
#include "GameEventMgr.h"

/* ContentData
npc_air_force_bots       80%    support for misc (invisible) guard bots in areas where player allowed to fly. Summon guards after a preset time if tagged by spell
npc_chicken_cluck       100%    support for quest 3861 (Cluck!)
npc_dancing_flames      100%    midsummer event NPC
npc_guardian            100%    guardianAI used to prevent players from accessing off-limits areas. Not in use by SD2
npc_garments_of_quests   80%    NPC's related to all Garments of-quests 5621, 5624, 5625, 5648, 5650
npc_injured_patient     100%    patients for triage-quests (6622 and 6624)
npc_doctor              100%    Gustaf Vanhowzen and Gregory Victor, quest 6622 and 6624 (Triage)
npc_innkeeper            25%    ScriptName not assigned. Innkeepers in general.
npc_mount_vendor        100%    Regular mount vendors all over the world. Display gossip if player doesn't meet the requirements to buy
npc_sayge               100%    Darkmoon event fortune teller, buff player based on answers given
npc_tabard_vendor        50%    allow recovering quest related tabards, achievement related ones need core support
npc_locksmith            75%    list of keys needs to be confirmed
npc_death_knight_gargoyle       AI for summoned gargoyle of deathknights
npc_horseman_fire_bunny
npc_shade_of_horseman
EndContentData */

/*########
# npc_air_force_bots
#########*/

enum SpawnType
{
    SPAWNTYPE_TRIPWIRE_ROOFTOP,                             // no warning, summon creature at smaller range
    SPAWNTYPE_ALARMBOT                                      // cast guards mark and summon npc - if player shows up with that buff duration < 5 seconds attack
};

struct SpawnAssociation
{
    uint32 m_uiThisCreatureEntry;
    uint32 m_uiSpawnedCreatureEntry;
    SpawnType m_SpawnType;
};

enum
{
    SPELL_GUARDS_MARK               = 38067,
    AURA_DURATION_TIME_LEFT         = 5000
};

const float RANGE_TRIPWIRE          = 15.0f;
const float RANGE_GUARDS_MARK       = 50.0f;

SpawnAssociation m_aSpawnAssociations[] =
{
    {2614,  15241, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Alliance)
    {2615,  15242, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Horde)
    {21974, 21976, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Area 52)
    {21993, 15242, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Horde - Bat Rider)
    {21996, 15241, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Alliance - Gryphon)
    {21997, 21976, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Goblin - Area 52 - Zeppelin)
    {21999, 15241, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Alliance)
    {22001, 15242, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Horde)
    {22002, 15242, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Ground (Horde)
    {22003, 15241, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Ground (Alliance)
    {22063, 21976, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Goblin - Area 52)
    {22065, 22064, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Ethereal - Stormspire)
    {22066, 22067, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Scryer - Dragonhawk)
    {22068, 22064, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Ethereal - Stormspire)
    {22069, 22064, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Stormspire)
    {22070, 22067, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Scryer)
    {22071, 22067, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Scryer)
    {22078, 22077, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Aldor)
    {22079, 22077, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Aldor - Gryphon)
    {22080, 22077, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Aldor)
    {22086, 22085, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Sporeggar)
    {22087, 22085, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Sporeggar - Spore Bat)
    {22088, 22085, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Sporeggar)
    {22090, 22089, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Toshley's Station - Flying Machine)
    {22124, 22122, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Cenarion)
    {22125, 22122, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Cenarion - Stormcrow)
    {22126, 22122, SPAWNTYPE_ALARMBOT}                      //Air Force Trip Wire - Rooftop (Cenarion Expedition)
};

struct MANGOS_DLL_DECL npc_air_force_botsAI : public ScriptedAI
{
    npc_air_force_botsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pSpawnAssoc = NULL;

        // find the correct spawnhandling
        static uint32 uiEntryCount = sizeof(m_aSpawnAssociations)/sizeof(SpawnAssociation);

        for (uint8 i=0; i<uiEntryCount; ++i)
        {
            if (m_aSpawnAssociations[i].m_uiThisCreatureEntry == pCreature->GetEntry())
            {
                m_pSpawnAssoc = &m_aSpawnAssociations[i];
                break;
            }
        }

        if (!m_pSpawnAssoc)
            error_db_log("SD2: Creature template entry %u has ScriptName npc_air_force_bots, but it's not handled by that script", pCreature->GetEntry());
        else
        {
            CreatureInfo const* spawnedTemplate = GetCreatureTemplateStore(m_pSpawnAssoc->m_uiSpawnedCreatureEntry);

            if (!spawnedTemplate)
            {
                error_db_log("SD2: Creature template entry %u does not exist in DB, which is required by npc_air_force_bots", m_pSpawnAssoc->m_uiSpawnedCreatureEntry);
                m_pSpawnAssoc = NULL;
                return;
            }
        }
    }

    SpawnAssociation* m_pSpawnAssoc;
    ObjectGuid m_spawnedGuid;

    void Reset() { }

    Creature* SummonGuard()
    {
        Creature* pSummoned = m_creature->SummonCreature(m_pSpawnAssoc->m_uiSpawnedCreatureEntry, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 300000);

        if (pSummoned)
            m_spawnedGuid = pSummoned->GetObjectGuid();
        else
        {
            error_db_log("SD2: npc_air_force_bots: wasn't able to spawn creature %u", m_pSpawnAssoc->m_uiSpawnedCreatureEntry);
            m_pSpawnAssoc = NULL;
        }

        return pSummoned;
    }

    Creature* GetSummonedGuard()
    {
        Creature* pCreature = m_creature->GetMap()->GetCreature(m_spawnedGuid);

        if (pCreature && pCreature->isAlive())
            return pCreature;

        return NULL;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_pSpawnAssoc)
            return;

        if (pWho->isTargetableForAttack() && m_creature->IsHostileTo(pWho))
        {
            Player* pPlayerTarget = pWho->GetTypeId() == TYPEID_PLAYER ? (Player*)pWho : NULL;

            // airforce guards only spawn for players
            if (!pPlayerTarget)
                return;

            Creature* pLastSpawnedGuard = m_spawnedGuid ? GetSummonedGuard() : NULL;

            // prevent calling GetCreature at next MoveInLineOfSight call - speedup
            if (!pLastSpawnedGuard)
                m_spawnedGuid.Clear();

            switch(m_pSpawnAssoc->m_SpawnType)
            {
                case SPAWNTYPE_ALARMBOT:
                {
                    if (!pWho->IsWithinDistInMap(m_creature, RANGE_GUARDS_MARK))
                        return;

                    Aura* pMarkAura = pWho->GetAura(SPELL_GUARDS_MARK, EFFECT_INDEX_0);
                    if (pMarkAura)
                    {
                        // the target wasn't able to move out of our range within 25 seconds
                        if (!pLastSpawnedGuard)
                        {
                            pLastSpawnedGuard = SummonGuard();

                            if (!pLastSpawnedGuard)
                                return;
                        }

                        if (pMarkAura->GetAuraDuration() < AURA_DURATION_TIME_LEFT)
                        {
                            if (!pLastSpawnedGuard->getVictim())
                                pLastSpawnedGuard->AI()->AttackStart(pWho);
                        }
                    }
                    else
                    {
                        if (!pLastSpawnedGuard)
                            pLastSpawnedGuard = SummonGuard();

                        if (!pLastSpawnedGuard)
                            return;

                        pLastSpawnedGuard->CastSpell(pWho, SPELL_GUARDS_MARK, true);
                    }
                    break;
                }
                case SPAWNTYPE_TRIPWIRE_ROOFTOP:
                {
                    if (!pWho->IsWithinDistInMap(m_creature, RANGE_TRIPWIRE))
                        return;

                    if (!pLastSpawnedGuard)
                        pLastSpawnedGuard = SummonGuard();

                    if (!pLastSpawnedGuard)
                        return;

                    // ROOFTOP only triggers if the player is on the ground
                    if (!pPlayerTarget->IsFlying())
                    {
                        if (!pLastSpawnedGuard->getVictim())
                            pLastSpawnedGuard->AI()->AttackStart(pWho);
                    }
                    break;
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_air_force_bots(Creature* pCreature)
{
    return new npc_air_force_botsAI(pCreature);
}

/*########
# npc_chicken_cluck
#########*/

enum
{
    EMOTE_A_HELLO           = -1000204,
    EMOTE_H_HELLO           = -1000205,
    EMOTE_CLUCK_TEXT2       = -1000206,

    QUEST_CLUCK             = 3861,
    FACTION_FRIENDLY        = 35,
    FACTION_CHICKEN         = 31
};

struct MANGOS_DLL_DECL npc_chicken_cluckAI : public ScriptedAI
{
    npc_chicken_cluckAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiResetFlagTimer;

    void Reset()
    {
        m_uiResetFlagTimer = 120000;

        m_creature->setFaction(FACTION_CHICKEN);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }

    void ReceiveEmote(Player* pPlayer, uint32 uiEmote)
    {
        if (uiEmote == TEXTEMOTE_CHICKEN)
        {
            if (!urand(0, 29))
            {
                if (pPlayer->GetQuestStatus(QUEST_CLUCK) == QUEST_STATUS_NONE)
                {
                    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    m_creature->setFaction(FACTION_FRIENDLY);

                    DoScriptText(EMOTE_A_HELLO, m_creature);

                    /* are there any difference in texts, after 3.x ?
                    if (pPlayer->GetTeam() == HORDE)
                        DoScriptText(EMOTE_H_HELLO, m_creature);
                    else
                        DoScriptText(EMOTE_A_HELLO, m_creature);
                    */
                }
            }
        }

        if (uiEmote == TEXTEMOTE_CHEER)
        {
            if (pPlayer->GetQuestStatus(QUEST_CLUCK) == QUEST_STATUS_COMPLETE)
            {
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                m_creature->setFaction(FACTION_FRIENDLY);
                DoScriptText(EMOTE_CLUCK_TEXT2, m_creature);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Reset flags after a certain time has passed so that the next player has to start the 'event' again
        if (m_creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
        {
            if (m_uiResetFlagTimer < uiDiff)
                EnterEvadeMode();
            else
                m_uiResetFlagTimer -= uiDiff;
        }

        if (m_creature->SelectHostileTarget() && m_creature->getVictim())
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_chicken_cluck(Creature* pCreature)
{
    return new npc_chicken_cluckAI(pCreature);
}

bool QuestAccept_npc_chicken_cluck(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_CLUCK)
    {
        if (npc_chicken_cluckAI* pChickenAI = dynamic_cast<npc_chicken_cluckAI*>(pCreature->AI()))
            pChickenAI->Reset();
    }

    return true;
}

bool QuestRewarded_npc_chicken_cluck(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_CLUCK)
    {
        if (npc_chicken_cluckAI* pChickenAI = dynamic_cast<npc_chicken_cluckAI*>(pCreature->AI()))
            pChickenAI->Reset();
    }

    return true;
}

/*######
## npc_dancing_flames
######*/

enum
{
    SPELL_FIERY_SEDUCTION = 47057
};

struct MANGOS_DLL_DECL npc_dancing_flamesAI : public ScriptedAI
{
    npc_dancing_flamesAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() {}

    void ReceiveEmote(Player* pPlayer, uint32 uiEmote)
    {
        m_creature->SetFacingToObject(pPlayer);

        if (pPlayer->HasAura(SPELL_FIERY_SEDUCTION))
            pPlayer->RemoveAurasDueToSpell(SPELL_FIERY_SEDUCTION);

        if (pPlayer->IsMounted())
        {
            pPlayer->Unmount();                             // doesnt remove mount aura
            pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        }

        switch(uiEmote)
        {
            case TEXTEMOTE_DANCE: DoCastSpellIfCan(pPlayer, SPELL_FIERY_SEDUCTION); break;// dance -> cast SPELL_FIERY_SEDUCTION
            case TEXTEMOTE_WAVE:  m_creature->HandleEmote(EMOTE_ONESHOT_WAVE);      break;// wave -> wave
            case TEXTEMOTE_JOKE:  m_creature->HandleEmote(EMOTE_STATE_LAUGH);       break;// silly -> laugh(with sound)
            case TEXTEMOTE_BOW:   m_creature->HandleEmote(EMOTE_ONESHOT_BOW);       break;// bow -> bow
            case TEXTEMOTE_KISS:  m_creature->HandleEmote(TEXTEMOTE_CURTSEY);       break;// kiss -> curtsey
        }
    }
};

CreatureAI* GetAI_npc_dancing_flames(Creature* pCreature)
{
    return new npc_dancing_flamesAI(pCreature);
}

/*######
## Triage quest
######*/

enum
{
    SAY_DOC1                    = -1000201,
    SAY_DOC2                    = -1000202,
    SAY_DOC3                    = -1000203,

    QUEST_TRIAGE_H              = 6622,
    QUEST_TRIAGE_A              = 6624,

    DOCTOR_ALLIANCE             = 12939,
    DOCTOR_HORDE                = 12920,
    ALLIANCE_COORDS             = 7,
    HORDE_COORDS                = 6
};

struct Location
{
    float x, y, z, o;
};

static Location AllianceCoords[]=
{
    {-3757.38f, -4533.05f, 14.16f, 3.62f},                  // Top-far-right bunk as seen from entrance
    {-3754.36f, -4539.13f, 14.16f, 5.13f},                  // Top-far-left bunk
    {-3749.54f, -4540.25f, 14.28f, 3.34f},                  // Far-right bunk
    {-3742.10f, -4536.85f, 14.28f, 3.64f},                  // Right bunk near entrance
    {-3755.89f, -4529.07f, 14.05f, 0.57f},                  // Far-left bunk
    {-3749.51f, -4527.08f, 14.07f, 5.26f},                  // Mid-left bunk
    {-3746.37f, -4525.35f, 14.16f, 5.22f},                  // Left bunk near entrance
};

//alliance run to where
#define A_RUNTOX -3742.96f
#define A_RUNTOY -4531.52f
#define A_RUNTOZ 11.91f

static Location HordeCoords[]=
{
    {-1013.75f, -3492.59f, 62.62f, 4.34f},                  // Left, Behind
    {-1017.72f, -3490.92f, 62.62f, 4.34f},                  // Right, Behind
    {-1015.77f, -3497.15f, 62.82f, 4.34f},                  // Left, Mid
    {-1019.51f, -3495.49f, 62.82f, 4.34f},                  // Right, Mid
    {-1017.25f, -3500.85f, 62.98f, 4.34f},                  // Left, front
    {-1020.95f, -3499.21f, 62.98f, 4.34f}                   // Right, Front
};

//horde run to where
#define H_RUNTOX -1016.44f
#define H_RUNTOY -3508.48f
#define H_RUNTOZ 62.96f

const uint32 AllianceSoldierId[3] =
{
    12938,                                                  // 12938 Injured Alliance Soldier
    12936,                                                  // 12936 Badly injured Alliance Soldier
    12937                                                   // 12937 Critically injured Alliance Soldier
};

const uint32 HordeSoldierId[3] =
{
    12923,                                                  //12923 Injured Soldier
    12924,                                                  //12924 Badly injured Soldier
    12925                                                   //12925 Critically injured Soldier
};

/*######
## npc_doctor (handles both Gustaf Vanhowzen and Gregory Victor)
######*/

struct MANGOS_DLL_DECL npc_doctorAI : public ScriptedAI
{
    npc_doctorAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ObjectGuid m_playerGuid;

    uint32 SummonPatient_Timer;
    uint32 SummonPatientCount;
    uint32 PatientDiedCount;
    uint32 PatientSavedCount;

    bool Event;

    GUIDList Patients;
    std::vector<Location*> Coordinates;

    void Reset()
    {
        m_playerGuid.Clear();

        SummonPatient_Timer = 10000;
        SummonPatientCount = 0;
        PatientDiedCount = 0;
        PatientSavedCount = 0;

        Patients.clear();
        Coordinates.clear();

        Event = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void BeginEvent(Player* pPlayer);
    void PatientDied(Location* Point);
    void PatientSaved(Creature* soldier, Player* pPlayer, Location* Point);
    void UpdateAI(const uint32 diff);
};

/*#####
## npc_injured_patient (handles all the patients, no matter Horde or Alliance)
#####*/

struct MANGOS_DLL_DECL npc_injured_patientAI : public ScriptedAI
{
    npc_injured_patientAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ObjectGuid m_doctorGuid;
    Location* Coord;

    void Reset()
    {
        m_doctorGuid.Clear();
        Coord = NULL;

        //no select
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        //no regen health
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        //to make them lay with face down
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

        uint32 mobId = m_creature->GetEntry();

        switch (mobId)
        {                                                   //lower max health
            case 12923:
            case 12938:                                     //Injured Soldier
                m_creature->SetHealth(uint32(m_creature->GetMaxHealth()*.75));
                break;
            case 12924:
            case 12936:                                     //Badly injured Soldier
                m_creature->SetHealth(uint32(m_creature->GetMaxHealth()*.50));
                break;
            case 12925:
            case 12937:                                     //Critically injured Soldier
                m_creature->SetHealth(uint32(m_creature->GetMaxHealth()*.25));
                break;
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER && m_creature->isAlive() && spell->Id == 20804)
        {
            if ((((Player*)caster)->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (((Player*)caster)->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE))
            {
                if (m_doctorGuid)
                {
                    if (Creature* pDoctor = m_creature->GetMap()->GetCreature(m_doctorGuid))
                    {
                        if (npc_doctorAI* pDocAI = dynamic_cast<npc_doctorAI*>(pDoctor->AI()))
                            pDocAI->PatientSaved(m_creature, (Player*)caster, Coord);
                    }
                }
            }
            //make not selectable
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            //regen health
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            //stand up
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);

            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_DOC1,m_creature); break;
                case 1: DoScriptText(SAY_DOC2,m_creature); break;
                case 2: DoScriptText(SAY_DOC3,m_creature); break;
            }

            m_creature->SetWalk(false);

            uint32 mobId = m_creature->GetEntry();

            switch (mobId)
            {
                case 12923:
                case 12924:
                case 12925:
                    m_creature->GetMotionMaster()->MovePoint(0, H_RUNTOX, H_RUNTOY, H_RUNTOZ);
                    break;
                case 12936:
                case 12937:
                case 12938:
                    m_creature->GetMotionMaster()->MovePoint(0, A_RUNTOX, A_RUNTOY, A_RUNTOZ);
                    break;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //lower HP on every world tick makes it a useful counter, not officlone though
        uint32 uiHPLose = uint32(0.05f * diff);
        if (m_creature->isAlive() && m_creature->GetHealth() > 1 + uiHPLose)
        {
            m_creature->SetHealth(m_creature->GetHealth() - uiHPLose);
        }

        if (m_creature->isAlive() && m_creature->GetHealth() <= 1 + uiHPLose)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetDeathState(JUST_DIED);
            m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);

            if (m_doctorGuid)
            {
                if (Creature* pDoctor = m_creature->GetMap()->GetCreature(m_doctorGuid))
                {
                    if (npc_doctorAI* pDocAI = dynamic_cast<npc_doctorAI*>(pDoctor->AI()))
                        pDocAI->PatientDied(Coord);
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_injured_patient(Creature* pCreature)
{
    return new npc_injured_patientAI(pCreature);
}

/*
npc_doctor (continue)
*/

void npc_doctorAI::BeginEvent(Player* pPlayer)
{
    m_playerGuid = pPlayer->GetObjectGuid();

    SummonPatient_Timer = 10000;
    SummonPatientCount = 0;
    PatientDiedCount = 0;
    PatientSavedCount = 0;

    switch(m_creature->GetEntry())
    {
        case DOCTOR_ALLIANCE:
            for(uint8 i = 0; i < ALLIANCE_COORDS; ++i)
                Coordinates.push_back(&AllianceCoords[i]);
            break;
        case DOCTOR_HORDE:
            for(uint8 i = 0; i < HORDE_COORDS; ++i)
                Coordinates.push_back(&HordeCoords[i]);
            break;
    }

    Event = true;
    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void npc_doctorAI::PatientDied(Location* Point)
{
    Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid);

    if (pPlayer && ((pPlayer->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (pPlayer->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)))
    {
        ++PatientDiedCount;

        if (PatientDiedCount > 5 && Event)
        {
            if (pPlayer->GetQuestStatus(QUEST_TRIAGE_A) == QUEST_STATUS_INCOMPLETE)
                pPlayer->FailQuest(QUEST_TRIAGE_A);
            else if (pPlayer->GetQuestStatus(QUEST_TRIAGE_H) == QUEST_STATUS_INCOMPLETE)
                pPlayer->FailQuest(QUEST_TRIAGE_H);

            Reset();
            return;
        }

        Coordinates.push_back(Point);
    }
    else
        // If no player or player abandon quest in progress
        Reset();
}

void npc_doctorAI::PatientSaved(Creature* soldier, Player* pPlayer, Location* Point)
{
    if (pPlayer && m_playerGuid == pPlayer->GetObjectGuid())
    {
        if ((pPlayer->GetQuestStatus(QUEST_TRIAGE_A) == QUEST_STATUS_INCOMPLETE) || (pPlayer->GetQuestStatus(QUEST_TRIAGE_H) == QUEST_STATUS_INCOMPLETE))
        {
            ++PatientSavedCount;

            if (PatientSavedCount == 15)
            {
                for(GUIDList::const_iterator itr = Patients.begin(); itr != Patients.end(); ++itr)
                {
                    if (Creature* Patient = m_creature->GetMap()->GetCreature(*itr))
                        Patient->SetDeathState(JUST_DIED);
                }

                if (pPlayer->GetQuestStatus(QUEST_TRIAGE_A) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->GroupEventHappens(QUEST_TRIAGE_A, m_creature);
                else if (pPlayer->GetQuestStatus(QUEST_TRIAGE_H) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->GroupEventHappens(QUEST_TRIAGE_H, m_creature);

                Reset();
                return;
            }

            Coordinates.push_back(Point);
        }
    }
}

void npc_doctorAI::UpdateAI(const uint32 diff)
{
    if (Event && SummonPatientCount >= 20)
    {
        Reset();
        return;
    }

    if (Event)
    {
        if (SummonPatient_Timer < diff)
        {
            Creature* Patient = NULL;
            Location* Point = NULL;

            if (Coordinates.empty())
                return;

            std::vector<Location*>::iterator itr = Coordinates.begin()+rand()%Coordinates.size();
            uint32 patientEntry = 0;

            switch(m_creature->GetEntry())
            {
                case DOCTOR_ALLIANCE: patientEntry = AllianceSoldierId[urand(0, 2)]; break;
                case DOCTOR_HORDE:    patientEntry = HordeSoldierId[urand(0, 2)]; break;
                default:
                    error_log("SD2: Invalid entry for Triage doctor. Please check your database");
                    return;
            }

            Point = *itr;

            Patient = m_creature->SummonCreature(patientEntry, Point->x, Point->y, Point->z, Point->o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);

            if (Patient)
            {
                //303, this flag appear to be required for client side item->spell to work (TARGET_SINGLE_FRIEND)
                Patient->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);

                Patients.push_back(Patient->GetObjectGuid());

                npc_injured_patientAI* pPatientAI = dynamic_cast<npc_injured_patientAI*>(Patient->AI());

                if (pPatientAI)
                {
                    pPatientAI->m_doctorGuid = m_creature->GetObjectGuid();

                    if (Point)
                        pPatientAI->Coord = Point;
                }

                Coordinates.erase(itr);
            }
            SummonPatient_Timer = 10000;
            ++SummonPatientCount;
        }else SummonPatient_Timer -= diff;
    }
}

bool QuestAccept_npc_doctor(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if ((pQuest->GetQuestId() == QUEST_TRIAGE_A) || (pQuest->GetQuestId() == QUEST_TRIAGE_H))
    {
        if (npc_doctorAI* pDocAI = dynamic_cast<npc_doctorAI*>(pCreature->AI()))
            pDocAI->BeginEvent(pPlayer);
    }

    return true;
}

CreatureAI* GetAI_npc_doctor(Creature* pCreature)
{
    return new npc_doctorAI(pCreature);
}

/*######
## npc_garments_of_quests
######*/

enum
{
    SPELL_LESSER_HEAL_R2    = 2052,
    SPELL_FORTITUDE_R1      = 1243,

    QUEST_MOON              = 5621,
    QUEST_LIGHT_1           = 5624,
    QUEST_LIGHT_2           = 5625,
    QUEST_SPIRIT            = 5648,
    QUEST_DARKNESS          = 5650,

    ENTRY_SHAYA             = 12429,
    ENTRY_ROBERTS           = 12423,
    ENTRY_DOLF              = 12427,
    ENTRY_KORJA             = 12430,
    ENTRY_DG_KEL            = 12428,

    SAY_COMMON_HEALED       = -1000231,
    SAY_DG_KEL_THANKS       = -1000232,
    SAY_DG_KEL_GOODBYE      = -1000233,
    SAY_ROBERTS_THANKS      = -1000256,
    SAY_ROBERTS_GOODBYE     = -1000257,
    SAY_KORJA_THANKS        = -1000258,
    SAY_KORJA_GOODBYE       = -1000259,
    SAY_DOLF_THANKS         = -1000260,
    SAY_DOLF_GOODBYE        = -1000261,
    SAY_SHAYA_THANKS        = -1000262,
    SAY_SHAYA_GOODBYE       = -1000263,
};

struct MANGOS_DLL_DECL npc_garments_of_questsAI : public npc_escortAI
{
    npc_garments_of_questsAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    ObjectGuid m_playerGuid;

    bool m_bIsHealed;
    bool m_bCanRun;

    uint32 m_uiRunAwayTimer;

    void Reset()
    {
        m_playerGuid.Clear();

        m_bIsHealed = false;
        m_bCanRun = false;

        m_uiRunAwayTimer = 5000;

        m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
        // expect database to have RegenHealth=0
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*0.7));
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_LESSER_HEAL_R2 || pSpell->Id == SPELL_FORTITUDE_R1)
        {
            // not while in combat
            if (m_creature->isInCombat())
                return;

            // nothing to be done now
            if (m_bIsHealed && m_bCanRun)
                return;

            if (pCaster->GetTypeId() == TYPEID_PLAYER)
            {
                switch(m_creature->GetEntry())
                {
                    case ENTRY_SHAYA:
                        if (((Player*)pCaster)->GetQuestStatus(QUEST_MOON) == QUEST_STATUS_INCOMPLETE)
                        {
                            if (m_bIsHealed && !m_bCanRun && pSpell->Id == SPELL_FORTITUDE_R1)
                            {
                                DoScriptText(SAY_SHAYA_THANKS, m_creature, pCaster);
                                m_bCanRun = true;
                            }
                            else if (!m_bIsHealed && pSpell->Id == SPELL_LESSER_HEAL_R2)
                            {
                                m_playerGuid = pCaster->GetObjectGuid();
                                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                                DoScriptText(SAY_COMMON_HEALED, m_creature, pCaster);
                                m_bIsHealed = true;
                            }
                        }
                        break;
                    case ENTRY_ROBERTS:
                        if (((Player*)pCaster)->GetQuestStatus(QUEST_LIGHT_1) == QUEST_STATUS_INCOMPLETE)
                        {
                            if (m_bIsHealed && !m_bCanRun && pSpell->Id == SPELL_FORTITUDE_R1)
                            {
                                DoScriptText(SAY_ROBERTS_THANKS, m_creature, pCaster);
                                m_bCanRun = true;
                            }
                            else if (!m_bIsHealed && pSpell->Id == SPELL_LESSER_HEAL_R2)
                            {
                                m_playerGuid = pCaster->GetObjectGuid();
                                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                                DoScriptText(SAY_COMMON_HEALED, m_creature, pCaster);
                                m_bIsHealed = true;
                            }
                        }
                        break;
                    case ENTRY_DOLF:
                        if (((Player*)pCaster)->GetQuestStatus(QUEST_LIGHT_2) == QUEST_STATUS_INCOMPLETE)
                        {
                            if (m_bIsHealed && !m_bCanRun && pSpell->Id == SPELL_FORTITUDE_R1)
                            {
                                DoScriptText(SAY_DOLF_THANKS, m_creature, pCaster);
                                m_bCanRun = true;
                            }
                            else if (!m_bIsHealed && pSpell->Id == SPELL_LESSER_HEAL_R2)
                            {
                                m_playerGuid = pCaster->GetObjectGuid();
                                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                                DoScriptText(SAY_COMMON_HEALED, m_creature, pCaster);
                                m_bIsHealed = true;
                            }
                        }
                        break;
                    case ENTRY_KORJA:
                        if (((Player*)pCaster)->GetQuestStatus(QUEST_SPIRIT) == QUEST_STATUS_INCOMPLETE)
                        {
                            if (m_bIsHealed && !m_bCanRun && pSpell->Id == SPELL_FORTITUDE_R1)
                            {
                                DoScriptText(SAY_KORJA_THANKS, m_creature, pCaster);
                                m_bCanRun = true;
                            }
                            else if (!m_bIsHealed && pSpell->Id == SPELL_LESSER_HEAL_R2)
                            {
                                m_playerGuid = pCaster->GetObjectGuid();
                                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                                DoScriptText(SAY_COMMON_HEALED, m_creature, pCaster);
                                m_bIsHealed = true;
                            }
                        }
                        break;
                    case ENTRY_DG_KEL:
                        if (((Player*)pCaster)->GetQuestStatus(QUEST_DARKNESS) == QUEST_STATUS_INCOMPLETE)
                        {
                            if (m_bIsHealed && !m_bCanRun && pSpell->Id == SPELL_FORTITUDE_R1)
                            {
                                DoScriptText(SAY_DG_KEL_THANKS, m_creature, pCaster);
                                m_bCanRun = true;
                            }
                            else if (!m_bIsHealed && pSpell->Id == SPELL_LESSER_HEAL_R2)
                            {
                                m_playerGuid = pCaster->GetObjectGuid();
                                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                                DoScriptText(SAY_COMMON_HEALED, m_creature, pCaster);
                                m_bIsHealed = true;
                            }
                        }
                        break;
                }

                // give quest credit, not expect any special quest objectives
                if (m_bCanRun)
                    ((Player*)pCaster)->TalkedToCreature(m_creature->GetEntry(), m_creature->GetObjectGuid());
            }
        }
    }

    void WaypointReached(uint32 uiPointId) {}

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (m_bCanRun && !m_creature->isInCombat())
        {
            if (m_uiRunAwayTimer <= uiDiff)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                {
                    switch(m_creature->GetEntry())
                    {
                        case ENTRY_SHAYA:   DoScriptText(SAY_SHAYA_GOODBYE, m_creature, pPlayer);   break;
                        case ENTRY_ROBERTS: DoScriptText(SAY_ROBERTS_GOODBYE, m_creature, pPlayer); break;
                        case ENTRY_DOLF:    DoScriptText(SAY_DOLF_GOODBYE, m_creature, pPlayer);    break;
                        case ENTRY_KORJA:   DoScriptText(SAY_KORJA_GOODBYE, m_creature, pPlayer);   break;
                        case ENTRY_DG_KEL:  DoScriptText(SAY_DG_KEL_GOODBYE, m_creature, pPlayer);  break;
                    }

                    Start(true);
                }
                else
                    EnterEvadeMode();                       // something went wrong

                m_uiRunAwayTimer = 30000;
            }
            else
                m_uiRunAwayTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_garments_of_quests(Creature* pCreature)
{
    return new npc_garments_of_questsAI(pCreature);
}

/*######
## npc_guardian
######*/

#define SPELL_DEATHTOUCH                5

struct MANGOS_DLL_DECL npc_guardianAI : public ScriptedAI
{
    npc_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->isAttackReady())
        {
            m_creature->CastSpell(m_creature->getVictim(),SPELL_DEATHTOUCH, true);
            m_creature->resetAttackTimer();
        }
    }
};

CreatureAI* GetAI_npc_guardian(Creature* pCreature)
{
    return new npc_guardianAI(pCreature);
}

/*########
# npc_innkeeper
#########*/

// Script applied to all innkeepers by npcflag.
// Are there any known innkeepers that does not hape the options in the below?
// (remember gossipHello is not called unless npcflag|1 is present)

enum
{
    TEXT_ID_WHAT_TO_DO              = 1853,

    SPELL_TRICK_OR_TREAT            = 24751,                 // create item or random buff
    SPELL_TRICK_OR_TREATED          = 24755,                 // buff player get when tricked or treated
};

#define GOSSIP_ITEM_TRICK_OR_TREAT  "Trick or Treat!"
#define GOSSIP_ITEM_WHAT_TO_DO      "What can I do at an Inn?"

bool GossipHello_npc_innkeeper(Player* pPlayer, Creature* pCreature)
{
    pPlayer->PrepareGossipMenu(pCreature, pPlayer->GetDefaultGossipMenuForSource(pCreature));

    if (IsHolidayActive(HOLIDAY_HALLOWS_END) && !pPlayer->HasAura(SPELL_TRICK_OR_TREATED, EFFECT_INDEX_0))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TRICK_OR_TREAT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    // Should only apply to innkeeper close to start areas.
    if (AreaTableEntry const* pAreaEntry = GetAreaEntryByAreaID(pCreature->GetAreaId()))
    {
        if (pAreaEntry->flags & AREA_FLAG_LOWLEVEL)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_WHAT_TO_DO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }

    pPlayer->TalkedToCreature(pCreature->GetEntry(), pCreature->GetObjectGuid());
    pPlayer->SendPreparedGossip(pCreature);
    return true;
}

bool GossipSelect_npc_innkeeper(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_WHAT_TO_DO, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer, SPELL_TRICK_OR_TREAT, true);
            break;
        case GOSSIP_OPTION_VENDOR:
            pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());
            break;
        case GOSSIP_OPTION_INNKEEPER:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->SetBindPoint(pCreature->GetObjectGuid());
            break;
    }

    return true;
}

/*######
## npc_mount_vendor
######*/

bool GossipHello_npc_mount_vendor(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    bool canBuy;
    canBuy = false;
    uint32 vendor = pCreature->GetEntry();
    uint8 race = pPlayer->getRace();

    switch (vendor)
    {
        case 384:                                           //Katie Hunter
        case 1460:                                          //Unger Statforth
        case 2357:                                          //Merideth Carlson
        case 4885:                                          //Gregor MacVince
            if (pPlayer->GetReputationRank(72) != REP_EXALTED && race != RACE_HUMAN)
                pPlayer->SEND_GOSSIP_MENU(5855, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 1261:                                          //Veron Amberstill
            if (pPlayer->GetReputationRank(47) != REP_EXALTED && race != RACE_DWARF)
                pPlayer->SEND_GOSSIP_MENU(5856, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 3362:                                          //Ogunaro Wolfrunner
            if (pPlayer->GetReputationRank(76) != REP_EXALTED && race != RACE_ORC)
                pPlayer->SEND_GOSSIP_MENU(5841, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 3685:                                          //Harb Clawhoof
            if (pPlayer->GetReputationRank(81) != REP_EXALTED && race != RACE_TAUREN)
                pPlayer->SEND_GOSSIP_MENU(5843, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 4730:                                          //Lelanai
            if (pPlayer->GetReputationRank(69) != REP_EXALTED && race != RACE_NIGHTELF)
                pPlayer->SEND_GOSSIP_MENU(5844, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 4731:                                          //Zachariah Post
            if (pPlayer->GetReputationRank(68) != REP_EXALTED && race != RACE_UNDEAD)
                pPlayer->SEND_GOSSIP_MENU(5840, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 7952:                                          //Zjolnir
            if (pPlayer->GetReputationRank(530) != REP_EXALTED && race != RACE_TROLL)
                pPlayer->SEND_GOSSIP_MENU(5842, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 7955:                                          //Milli Featherwhistle
            if (pPlayer->GetReputationRank(54) != REP_EXALTED && race != RACE_GNOME)
                pPlayer->SEND_GOSSIP_MENU(5857, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 16264:                                         //Winaestra
            if (pPlayer->GetReputationRank(911) != REP_EXALTED && race != RACE_BLOODELF)
                pPlayer->SEND_GOSSIP_MENU(10305, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
        case 17584:                                         //Torallius the Pack Handler
            if (pPlayer->GetReputationRank(930) != REP_EXALTED && race != RACE_DRAENEI)
                pPlayer->SEND_GOSSIP_MENU(10239, pCreature->GetObjectGuid());
            else canBuy = true;
            break;
    }

    if (canBuy)
    {
        if (pCreature->isVendor())
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    }
    return true;
}

bool GossipSelect_npc_mount_vendor(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());

    return true;
}

/*######
## npc_sayge
######*/

#define SPELL_DMG       23768                               //dmg
#define SPELL_RES       23769                               //res
#define SPELL_ARM       23767                               //arm
#define SPELL_SPI       23738                               //spi
#define SPELL_INT       23766                               //int
#define SPELL_STM       23737                               //stm
#define SPELL_STR       23735                               //str
#define SPELL_AGI       23736                               //agi
#define SPELL_FORTUNE   23765                               //faire fortune

bool GossipHello_npc_sayge(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->HasSpellCooldown(SPELL_INT) ||
        pPlayer->HasSpellCooldown(SPELL_ARM) ||
        pPlayer->HasSpellCooldown(SPELL_DMG) ||
        pPlayer->HasSpellCooldown(SPELL_RES) ||
        pPlayer->HasSpellCooldown(SPELL_STR) ||
        pPlayer->HasSpellCooldown(SPELL_AGI) ||
        pPlayer->HasSpellCooldown(SPELL_STM) ||
        pPlayer->HasSpellCooldown(SPELL_SPI))
        pPlayer->SEND_GOSSIP_MENU(7393, pCreature->GetObjectGuid());
    else
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(7339, pCreature->GetObjectGuid());
    }

    return true;
}

void SendAction_npc_sayge(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Slay the Man",                      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Turn him over to liege",            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Confiscate the corn",               GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Let him go and have the corn",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(7340, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Execute your friend painfully",     GOSSIP_SENDER_MAIN+1, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Execute your friend painlessly",    GOSSIP_SENDER_MAIN+2, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Let your friend go",                GOSSIP_SENDER_MAIN+3, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(7341, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Confront the diplomat",             GOSSIP_SENDER_MAIN+4, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Show not so quiet defiance",        GOSSIP_SENDER_MAIN+5, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Remain quiet",                      GOSSIP_SENDER_MAIN+2, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(7361, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Speak against your brother openly", GOSSIP_SENDER_MAIN+6, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Help your brother in",              GOSSIP_SENDER_MAIN+7, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Keep your brother out without letting him know", GOSSIP_SENDER_MAIN+8, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(7362, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Take credit, keep gold",            GOSSIP_SENDER_MAIN+5, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Take credit, share the gold",       GOSSIP_SENDER_MAIN+4, GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Let the knight take credit",        GOSSIP_SENDER_MAIN+3, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(7363, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Thanks",                            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            pPlayer->SEND_GOSSIP_MENU(7364, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pCreature->CastSpell(pPlayer, SPELL_FORTUNE, false);
            pPlayer->SEND_GOSSIP_MENU(7365, pCreature->GetObjectGuid());
            break;
    }
}

bool GossipSelect_npc_sayge(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiSender)
    {
        case GOSSIP_SENDER_MAIN:
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+1:
            pCreature->CastSpell(pPlayer, SPELL_DMG, false);
            pPlayer->AddSpellCooldown(SPELL_DMG,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+2:
            pCreature->CastSpell(pPlayer, SPELL_RES, false);
            pPlayer->AddSpellCooldown(SPELL_RES,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+3:
            pCreature->CastSpell(pPlayer, SPELL_ARM, false);
            pPlayer->AddSpellCooldown(SPELL_ARM,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+4:
            pCreature->CastSpell(pPlayer, SPELL_SPI, false);
            pPlayer->AddSpellCooldown(SPELL_SPI,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+5:
            pCreature->CastSpell(pPlayer, SPELL_INT, false);
            pPlayer->AddSpellCooldown(SPELL_INT,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+6:
            pCreature->CastSpell(pPlayer, SPELL_STM, false);
            pPlayer->AddSpellCooldown(SPELL_STM,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+7:
            pCreature->CastSpell(pPlayer, SPELL_STR, false);
            pPlayer->AddSpellCooldown(SPELL_STR,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
        case GOSSIP_SENDER_MAIN+8:
            pCreature->CastSpell(pPlayer, SPELL_AGI, false);
            pPlayer->AddSpellCooldown(SPELL_AGI,0,time(NULL) + 7200);
            SendAction_npc_sayge(pPlayer, pCreature, uiAction);
            break;
    }
    return true;
}

/*######
## npc_tabard_vendor
######*/

enum
{
    QUEST_TRUE_MASTERS_OF_LIGHT                = 9737,
    QUEST_THE_UNWRITTEN_PROPHECY               = 9762,
    QUEST_INTO_THE_BREACH                      = 10259,
    QUEST_BATTLE_OF_THE_CRIMSON_WATCH          = 10781,
    QUEST_SHARDS_OF_AHUNE                      = 11972,

    ACHIEVEMENT_EXPLORE_NORTHREND              = 45,
    ACHIEVEMENT_TWENTYFIVE_TABARDS             = 1021,
    ACHIEVEMENT_THE_LOREMASTER_A               = 1681,
    ACHIEVEMENT_THE_LOREMASTER_H               = 1682,

    ITEM_TABARD_OF_THE_HAND                    = 24344,
    ITEM_TABARD_OF_THE_BLOOD_KNIGHT            = 25549,
    ITEM_TABARD_OF_THE_PROTECTOR               = 28788,
    ITEM_OFFERING_OF_THE_SHATAR                = 31408,
    ITEM_GREEN_TROPHY_TABARD_OF_THE_ILLIDARI   = 31404,
    ITEM_PURPLE_TROPHY_TABARD_OF_THE_ILLIDARI  = 31405,
    ITEM_TABARD_OF_THE_SUMMER_SKIES            = 35279,
    ITEM_TABARD_OF_THE_SUMMER_FLAMES           = 35280,
    ITEM_TABARD_OF_THE_ACHIEVER                = 40643,
    ITEM_LOREMASTERS_COLORS                    = 43300,
    ITEM_TABARD_OF_THE_EXPLORER                = 43348,

    SPELL_TABARD_OF_THE_BLOOD_KNIGHT           = 54974,
    SPELL_TABARD_OF_THE_HAND                   = 54976,
    SPELL_GREEN_TROPHY_TABARD_OF_THE_ILLIDARI  = 54977,
    SPELL_PURPLE_TROPHY_TABARD_OF_THE_ILLIDARI = 54982,
    SPELL_TABARD_OF_THE_ACHIEVER               = 55006,
    SPELL_TABARD_OF_THE_PROTECTOR              = 55008,
    SPELL_LOREMASTERS_COLORS                   = 58194,
    SPELL_TABARD_OF_THE_EXPLORER               = 58224,
    SPELL_TABARD_OF_SUMMER_SKIES               = 62768,
    SPELL_TABARD_OF_SUMMER_FLAMES              = 62769
};

#define GOSSIP_LOST_TABARD_OF_BLOOD_KNIGHT               "I've lost my Tabard of Blood Knight."
#define GOSSIP_LOST_TABARD_OF_THE_HAND                   "I've lost my Tabard of the Hand."
#define GOSSIP_LOST_TABARD_OF_THE_PROTECTOR              "I've lost my Tabard of the Protector."
#define GOSSIP_LOST_GREEN_TROPHY_TABARD_OF_THE_ILLIDARI  "I've lost my Green Trophy Tabard of the Illidari."
#define GOSSIP_LOST_PURPLE_TROPHY_TABARD_OF_THE_ILLIDARI "I've lost my Purple Trophy Tabard of the Illidari."
#define GOSSIP_LOST_TABARD_OF_SUMMER_SKIES               "I've lost my Tabard of Summer Skies."
#define GOSSIP_LOST_TABARD_OF_SUMMER_FLAMES              "I've lost my Tabard of Summer Flames."
#define GOSSIP_LOST_LOREMASTERS_COLORS                   "I've lost my Loremaster's Colors."
#define GOSSIP_LOST_TABARD_OF_THE_EXPLORER               "I've lost my Tabard of the Explorer."
#define GOSSIP_LOST_TABARD_OF_THE_ACHIEVER               "I've lost my Tabard of the Achiever."

bool GossipHello_npc_tabard_vendor(Player* pPlayer, Creature* pCreature)
{
    bool m_bLostBloodKnight = false;
    bool m_bLostHand        = false;
    bool m_bLostProtector   = false;
    bool m_bLostIllidari    = false;
    bool m_bLostSummer      = false;

    //Tabard of the Blood Knight
    if (pPlayer->GetQuestRewardStatus(QUEST_TRUE_MASTERS_OF_LIGHT) && !pPlayer->HasItemCount(ITEM_TABARD_OF_THE_BLOOD_KNIGHT, 1, true))
        m_bLostBloodKnight = true;

    //Tabard of the Hand
    if (pPlayer->GetQuestRewardStatus(QUEST_THE_UNWRITTEN_PROPHECY) && !pPlayer->HasItemCount(ITEM_TABARD_OF_THE_HAND, 1, true))
        m_bLostHand = true;

    //Tabard of the Protector
    if (pPlayer->GetQuestRewardStatus(QUEST_INTO_THE_BREACH) && !pPlayer->HasItemCount(ITEM_TABARD_OF_THE_PROTECTOR, 1, true))
        m_bLostProtector = true;

    //Green Trophy Tabard of the Illidari
    //Purple Trophy Tabard of the Illidari
    if (pPlayer->GetQuestRewardStatus(QUEST_BATTLE_OF_THE_CRIMSON_WATCH) &&
        (!pPlayer->HasItemCount(ITEM_GREEN_TROPHY_TABARD_OF_THE_ILLIDARI, 1, true) &&
        !pPlayer->HasItemCount(ITEM_PURPLE_TROPHY_TABARD_OF_THE_ILLIDARI, 1, true) &&
        !pPlayer->HasItemCount(ITEM_OFFERING_OF_THE_SHATAR, 1, true)))
        m_bLostIllidari = true;

    //Tabard of Summer Skies
    //Tabard of Summer Flames
    if (pPlayer->GetQuestRewardStatus(QUEST_SHARDS_OF_AHUNE) &&
        !pPlayer->HasItemCount(ITEM_TABARD_OF_THE_SUMMER_SKIES, 1, true) &&
        !pPlayer->HasItemCount(ITEM_TABARD_OF_THE_SUMMER_FLAMES, 1, true))
        m_bLostSummer = true;

    if (m_bLostBloodKnight || m_bLostHand || m_bLostProtector || m_bLostIllidari || m_bLostSummer)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        if (m_bLostBloodKnight)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_TABARD_OF_BLOOD_KNIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF +1);

        if (m_bLostHand)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_TABARD_OF_THE_HAND, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF +2);

        if (m_bLostProtector)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_TABARD_OF_THE_PROTECTOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

        if (m_bLostIllidari)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_GREEN_TROPHY_TABARD_OF_THE_ILLIDARI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_PURPLE_TROPHY_TABARD_OF_THE_ILLIDARI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
        }

        if (m_bLostSummer)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_TABARD_OF_SUMMER_SKIES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_TABARD_OF_SUMMER_FLAMES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
        }

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    }
    else
        pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_tabard_vendor(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_TRADE:
            pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TABARD_OF_THE_BLOOD_KNIGHT, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TABARD_OF_THE_HAND, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TABARD_OF_THE_PROTECTOR, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_GREEN_TROPHY_TABARD_OF_THE_ILLIDARI, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_PURPLE_TROPHY_TABARD_OF_THE_ILLIDARI, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TABARD_OF_SUMMER_SKIES, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TABARD_OF_SUMMER_FLAMES, false);
            break;
    }
    return true;
}

/*######
## npc_mirror_image
######*/

enum MirrorImageSpells
{
    SPELL_CLONE_CASTER    = 45204,
    SPELL_CLONE_CASTER_1  = 69837,
//    SPELL_CLONE_CASTER_1  = 58836,
    SPELL_CLONE_THREAT    = 58838,
    SPELL_FIREBLAST       = 59637,
    SPELL_FROSTBOLT       = 59638,
    SPELL_FROSTSHIELD     = 43008,
    SPELL_FIRESHIELD      = 43046,
    SPELL_ICEBLOCK        = 65802,
    SPELL_ICERING         = 42917,
};

struct MANGOS_DLL_DECL npc_mirror_imageAI : public ScriptedAI
{
    npc_mirror_imageAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiFrostboltTimer;
    uint32 m_uiFrostringTimer;
    uint32 m_uiFireblastTimer;
    bool inCombat;
    Unit *owner;
    float angle;
    bool blocked;
    bool movement;

    void Reset() 
    {
        owner = m_creature->GetOwner();
        if (!owner) 
            return;

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_2,owner->GetUInt32Value(UNIT_FIELD_BYTES_2));
        m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
        m_creature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, DEFAULT_WORLD_OBJECT_SIZE);
        m_creature->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);

        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID,   owner->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID));
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, owner->GetUInt32Value(PLAYER_VISIBLE_ITEM_17_ENTRYID));
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+2, owner->GetUInt32Value(PLAYER_VISIBLE_ITEM_18_ENTRYID));
        m_creature->SetSpeedRate(MOVE_RUN, owner->GetSpeedRate(MOVE_RUN), true);

        m_uiFrostboltTimer = urand(0,3000);
        m_uiFrostringTimer = urand(2000,6000);
        m_uiFireblastTimer = urand(0,3000);
        inCombat = false;
        blocked = false;
        movement = false;


        if (owner && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
        {
            angle = m_creature->GetAngle(owner);
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST + 3.0f, angle);
        }

        if(owner->IsPvP())
            m_creature->SetPvP(true);
        if(owner->IsFFAPvP())
            m_creature->SetFFAPvP(true);
    }

    void AttackStart(Unit* pWho)
    {
      if (!pWho) return;

      if (m_creature->Attack(pWho, true))
        {
            if (owner)
                 m_creature->CastSpell(m_creature, SPELL_CLONE_THREAT, true, NULL, NULL, owner->GetObjectGuid());
            m_creature->clearUnitState(UNIT_STAT_FOLLOW);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            m_creature->AddThreat(pWho, 100.0f);
            DoStartMovement(pWho, 30.0f);
            SetCombatMovement(true);
            inCombat = true;
        }
    }

    void EnterEvadeMode()
    {
     if (m_creature->IsInEvadeMode() || !m_creature->isAlive())
          return;

        inCombat = false;

        m_creature->AttackStop();
        m_creature->CombatStop(true);
        if (owner && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
        {
            angle = m_creature->GetAngle(owner);
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST + 3.0f,angle);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!owner || !owner->isAlive()) 
            m_creature->ForcedDespawn();

        if (owner && !m_creature->HasAura(SPELL_CLONE_CASTER))
            m_creature->CastSpell(m_creature, SPELL_CLONE_CASTER, true, NULL, NULL, owner->GetObjectGuid());

        if (owner && !m_creature->HasAura(SPELL_CLONE_CASTER_1))
            m_creature->CastSpell(m_creature, SPELL_CLONE_CASTER_1, true, NULL, NULL, owner->GetObjectGuid());

        if (owner && owner->HasAura(SPELL_FROSTSHIELD) && !m_creature->HasAura(SPELL_FROSTSHIELD))
            m_creature->CastSpell(m_creature, SPELL_FROSTSHIELD, false);

        if (owner && owner->HasAura(SPELL_FIRESHIELD) && !m_creature->HasAura(SPELL_FIRESHIELD))
            m_creature->CastSpell(m_creature, SPELL_FIRESHIELD, false);

        if (!m_creature->getVictim())
            if (owner && owner->getVictim())
                AttackStart(owner->getVictim());

        if (m_creature->getVictim() && m_creature->getVictim() != owner->getVictim())
            AttackStart(owner->getVictim());

        if (inCombat && !m_creature->getVictim())
        {
            EnterEvadeMode();
            return;
        }

        if (!inCombat) 
            return;

        if (m_creature->IsWithinDistInMap(m_creature->getVictim(),30.0f))
        {
            movement = false;
            if (m_uiFrostboltTimer <= diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTBOLT);
                m_uiFrostboltTimer = urand(1000,6000);
            } else m_uiFrostboltTimer -= diff;

            if (m_uiFireblastTimer <= diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_FIREBLAST);
                m_uiFireblastTimer = urand(1000,6000);
            } else m_uiFireblastTimer -= diff;

            if (m_uiFrostringTimer <= diff && m_creature->IsWithinDistInMap(m_creature->getVictim(),5.0f))
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_ICERING);
                m_uiFrostringTimer = urand(4000,8000);
            } else m_uiFrostringTimer -= diff;

            if (!blocked && m_creature->GetHealthPercent() < 10.0f)
            {
                DoCastSpellIfCan(m_creature,SPELL_ICEBLOCK);
                blocked = true;
            }
        }
        else
            if (!movement) 
            {
                DoStartMovement(m_creature->getVictim(), 20.0f);
                movement = true;
            }

//        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_mirror_image(Creature* pCreature)
{
    return new npc_mirror_imageAI(pCreature);
};

/*####
 ## npc_snake_trap_serpents - Summonned snake id are 19921 and 19833
 ####*/

#define SPELL_MIND_NUMBING_POISON    25810   //Viper
#define SPELL_CRIPPLING_POISON       30981   //Viper
#define SPELL_DEADLY_POISON          34655   //Venomous Snake

#define MOB_VIPER 19921
#define MOB_VENOM_SNIKE 19833

struct MANGOS_DLL_DECL npc_snake_trap_serpentsAI : public ScriptedAI
{
    npc_snake_trap_serpentsAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SpellTimer;
    Unit* Owner;

    void Reset()
    {
        SpellTimer = 500;
        Owner = m_creature->GetCharmerOrOwner();
        if (!Owner) return;

        m_creature->SetLevel(Owner->getLevel());
        m_creature->setFaction(Owner->getFaction());
    }

    void AttackStart(Unit* pWho)
    {
      if (!pWho) return;

      if (m_creature->Attack(pWho, true))
         {
            m_creature->SetInCombatWith(pWho);
            m_creature->AddThreat(pWho, 100.0f);
            SetCombatMovement(true);
            m_creature->GetMotionMaster()->MoveChase(pWho);
         }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim())
        {
            if (Owner && Owner->getVictim())
                AttackStart(Owner->getVictim());
            return;
        }

        if (SpellTimer <= diff)
        {
            if (m_creature->GetEntry() == MOB_VIPER ) //Viper - 19921
            {
                if (!urand(0,2)) //33% chance to cast
                {
                    uint32 spell;
                    if (urand(0,1))
                        spell = SPELL_MIND_NUMBING_POISON;
                    else
                        spell = SPELL_CRIPPLING_POISON;
                    DoCast(m_creature->getVictim(), spell);
                }

                SpellTimer = urand(3000, 5000);
            }
            else if (m_creature->GetEntry() == MOB_VENOM_SNIKE ) //Venomous Snake - 19833
            {
                if (urand(0,1) == 0) //80% chance to cast
                    DoCast(m_creature->getVictim(), SPELL_DEADLY_POISON);
                SpellTimer = urand(2500, 4500);
            }
        }
        else SpellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_snake_trap_serpents(Creature* pCreature)
{
    return new npc_snake_trap_serpentsAI(pCreature);
}

struct MANGOS_DLL_DECL npc_rune_blade : public ScriptedAI
{
    npc_rune_blade(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    Unit* owner;

    void Reset()
    {
        owner = m_creature->GetOwner();
        if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
            return;

        // Cannot be Selected or Attacked
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        m_creature->SetLevel(owner->getLevel());
        m_creature->setFaction(owner->getFaction());

        // Add visible weapon
        if (Item const * item = ((Player *)owner)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, item->GetProto()->ItemId);

        // Add stats scaling
        int32 damageDone=owner->CalculateDamage(BASE_ATTACK, true); // might be average damage instead ?
        int32 meleeSpeed=owner->m_modAttackSpeedPct[BASE_ATTACK];
        m_creature->CastCustomSpell(m_creature, 51906, &damageDone, &meleeSpeed, NULL, true);

        // Visual Glow
        m_creature->CastSpell(m_creature, 53160, true);

        SetCombatMovement(true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!owner) return;

        if (!m_creature->getVictim())
        {
            if (owner->getVictim())
                AttackStart(owner->getVictim());
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_rune_blade(Creature* pCreature)
{
    return new npc_rune_blade(pCreature);
}

/*########
# mob_death_knight_gargoyle AI
#########*/

// UPDATE `creature_template` SET `ScriptName` = 'mob_death_knight_gargoyle' WHERE `entry` = '27829';

enum GargoyleSpells
{
    SPELL_GARGOYLE_STRIKE = 51963      // Don't know if this is the correct spell, it does about 700-800 damage points
};

struct MANGOS_DLL_DECL npc_death_knight_gargoyle : public ScriptedAI
{
    npc_death_knight_gargoyle(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }
    uint32 m_uiGargoyleStrikeTimer;
    bool inCombat;
    Unit *owner;


    void Reset() 
    {
     owner = m_creature->GetOwner();
     if (!owner) return;

     m_creature->SetLevel(owner->getLevel());
     m_creature->setFaction(owner->getFaction());

     m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
     m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
     m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
     m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
     m_creature->SetLevitate(true);

     inCombat = false;
     m_uiGargoyleStrikeTimer = urand(3000, 5000);

     float fPosX, fPosY, fPosZ;
     owner->GetPosition(fPosX, fPosY, fPosZ);

     m_creature->NearTeleportTo(fPosX, fPosY, fPosZ+10.0f, m_creature->GetAngle(owner));


     if (owner && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
        {
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST + 3.0f, m_creature->GetAngle(owner));
        }

      if(owner->IsPvP())
                 m_creature->SetPvP(true);
      if(owner->IsFFAPvP())
                 m_creature->SetFFAPvP(true);
    }

    void EnterEvadeMode()
    {
     if (m_creature->IsInEvadeMode() || !m_creature->isAlive())
          return;

        inCombat = false;

        m_creature->AttackStop();
        m_creature->CombatStop(true);
        if (owner && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
        {
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST + 3.0f, m_creature->GetAngle(owner));
        }
    }

    void AttackStart(Unit* pWho)
    {
      if (!pWho) return;

      if (m_creature->Attack(pWho, true))
        {
            m_creature->clearUnitState(UNIT_STAT_FOLLOW);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            m_creature->AddThreat(pWho, 100.0f);
            DoStartMovement(pWho, 10.0f);
            SetCombatMovement(true);
            inCombat = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!owner || !owner->IsInWorld())
        {
            m_creature->ForcedDespawn();
            return;
        }

        if (!m_creature->getVictim())
            if (owner && owner->getVictim())
                AttackStart(owner->getVictim());

        if (m_creature->getVictim() && m_creature->getVictim() != owner->getVictim())
                AttackStart(owner->getVictim());

        if (inCombat && !m_creature->getVictim())
        {
            EnterEvadeMode();
            return;
        }

        if (!inCombat) return;

        if (m_uiGargoyleStrikeTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_GARGOYLE_STRIKE);
            m_uiGargoyleStrikeTimer = urand(3000, 5000);
        }
        else m_uiGargoyleStrikeTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_death_knight_gargoyle(Creature* pCreature)
{
    return new npc_death_knight_gargoyle(pCreature);
}

/*########
# npc_risen_ally AI
#########*/

struct MANGOS_DLL_DECL npc_risen_allyAI : public ScriptedAI
{
    npc_risen_allyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
    }

    uint32 StartTimer;

    void Reset()
    {
        StartTimer = 2000;
        m_creature->SetSheath(SHEATH_STATE_MELEE);
        m_creature->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_ABANDONED);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
        m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
        m_creature->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);
        if (Player* creator = m_creature->GetMap()->GetPlayer(m_creature->GetCreatorGuid()))
        {
           m_creature->SetLevel(creator->getLevel());
           m_creature->setFaction(creator->getFaction());
        }
    }

    void JustDied(Unit* killer)
    {
        if (!m_creature)
            return;

        if (Player* creator = m_creature->GetMap()->GetPlayer(m_creature->GetCreatorGuid()))
        {
            creator->RemoveAurasDueToSpell(46619);
            creator->RemoveAurasDueToSpell(62218);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 10.0f);
            SetCombatMovement(true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(StartTimer > uiDiff)
        {
            StartTimer -= uiDiff;
            return;
        }

        if(!m_creature->isCharmed())
            m_creature->ForcedDespawn();

        if (m_creature->isInCombat())
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_risen_ally(Creature* pCreature)
{
    return new npc_risen_allyAI(pCreature);
}

/*########
# npc_explosive_decoyAI
#########*/

struct MANGOS_DLL_DECL npc_explosive_decoyAI : public ScriptedAI
{
    npc_explosive_decoyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    Player* p_owner;

    void Reset()
    {
        p_owner = NULL;
        SetCombatMovement(false);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;

        if (uiDamage > 0)
            m_creature->CastSpell(m_creature, 53273, true);
    }

    void JustDied(Unit* killer)
    {
        if (!m_creature || !p_owner)
            return;

        SpellEntry const* createSpell = GetSpellStore()->LookupEntry(m_creature->GetUInt32Value(UNIT_CREATED_BY_SPELL));

        if (createSpell)
            p_owner->SendCooldownEvent(createSpell);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (p_owner)
            return;

        p_owner = m_creature->GetMap()->GetPlayer(m_creature->GetCreatorGuid());

        if (!p_owner) 
            return;

        m_creature->setFaction(p_owner->getFaction());
        m_creature->SetCreatorGuid(ObjectGuid());
    }
};

CreatureAI* GetAI_npc_explosive_decoy(Creature* pCreature)
{
    return new npc_explosive_decoyAI(pCreature);
}

/*########
# npc_eye_of_kilroggAI
#########*/

struct MANGOS_DLL_DECL npc_eye_of_kilrogg : public ScriptedAI
{
    npc_eye_of_kilrogg(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    Player* p_owner;

    void Reset()
    {
        p_owner = NULL;
    }

    void UpdateAI(const uint32 diff)
    {
        if (p_owner)
            return;

        p_owner = (Player*)m_creature->GetCharmerOrOwner();

        if (!p_owner)
            return;

        if (!m_creature->HasAura(2585))
            m_creature->CastSpell(m_creature, 2585, true);

        if (p_owner->HasAura(58081))
            m_creature->CastSpell(m_creature, 58083, true);

    }
};

CreatureAI* GetAI_npc_eye_of_kilrogg(Creature* pCreature)
{
    return new npc_eye_of_kilrogg(pCreature);
}

/*########
# npc_fire_bunny
#########*/

enum
{
    NPC_FIRE_BUNNY          = 23686,
    SPELL_THROW_BUCKET      = 42339,
    SPELL_EXTINGUISH_VISUAL = 42348,
    SPELL_FLAMES_LARGE      = 42075,
    SPELL_SMOKE             = 42355,
    SPELL_CONFLAGRATE       = 42132,
    SPELL_HORSEMAN_MOUNT    = 48024,
    SPELL_HORSMAN_SHADE_VIS = 43904,
    SPELL_Q_STOP_THE_FIRE   = 42242,
    SPELL_Q_LET_THE_FIRES_C = 47775,
    SPELL_LAUGH_DELAYED_8   = 43893,

    PHASE_INITIAL           = 0,
    PHASE_1ST_SPEACH        = 1,
    PHASE_2ND_SPEACH        = 2,
    PHASE_FAIL              = 3,
    PHASE_END               = 4,

    YELL_IGNITE             = -1110001,
    YELL_1ST                = -1110002,
    YELL_2ND                = -1110003,
    YELL_FAIL               = -1110004,
    YELL_VICTORY            = -1110005,
    YELL_CONFLAGRATION      = -1110006
};

struct MANGOS_DLL_DECL npc_horseman_fire_bunnyAI : public Scripted_NoMovementAI
{
    npc_horseman_fire_bunnyAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        Reset();
        m_creature->RemoveAllAuras();
    }

    void Reset()
    {
        if (!m_creature->isAlive())
            m_creature->Respawn();
    }

    void SpellHit(Unit* pWho, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_THROW_BUCKET)
        {
            pWho->CastSpell(m_creature, SPELL_EXTINGUISH_VISUAL, false);
            m_creature->RemoveAurasDueToSpell(SPELL_FLAMES_LARGE);
        }
        if (pSpell->Id == SPELL_CONFLAGRATE)
        {
            DoCastSpellIfCan(m_creature, SPELL_FLAMES_LARGE);
            m_creature->RemoveAurasDueToSpell(SPELL_CONFLAGRATE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_creature->SelectHostileTarget() || m_creature->getVictim())
            EnterEvadeMode(); // Dunno how to prevent them from entering combat while hit by SPELL_EXTINGUISH_VISUAL (spelleffect 2)
    }
};

CreatureAI* GetAI_npc_horseman_fire_bunny(Creature* pCreature)
{
    return new npc_horseman_fire_bunnyAI (pCreature);
};

/*########
# npc_shade of horseman
#########*/

struct MANGOS_DLL_DECL npc_shade_of_horsemanAI : public ScriptedAI
{
    npc_shade_of_horsemanAI(Creature* pCreature) : ScriptedAI(pCreature){Reset();}

    uint8 uiPhase;
    uint32 m_uiEventTimer;
    uint32 m_uiConflagrationTimer;
    uint32 m_uiConflagrationProcTimer;
    bool bIsConflagrating;

    GUIDList lFireBunnies;

    void Reset()
    {
        if (!m_creature->isAlive())
            m_creature->Respawn();

        uiPhase = PHASE_INITIAL;
        lFireBunnies.clear();
        bIsConflagrating = true;

        m_uiEventTimer = 2.5*MINUTE*IN_MILLISECONDS;

        m_uiConflagrationTimer = 30000;
        m_uiConflagrationProcTimer = 2000;

        DoCastSpellIfCan(m_creature, SPELL_HORSEMAN_MOUNT);
        DoCastSpellIfCan(m_creature, SPELL_HORSMAN_SHADE_VIS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (uiPhase == PHASE_INITIAL)
        {
            DoScriptText(YELL_IGNITE, m_creature);
            ++uiPhase;
            return;
        }
        else if (uiPhase == PHASE_END)
            return;

        if (!bIsConflagrating)
        {
            bool IsVictory = true;
            for (GUIDList::iterator itr = lFireBunnies.begin(); itr != lFireBunnies.end(); ++itr)
                if (Creature* pFireBunny = m_creature->GetMap()->GetCreature(*itr))
                    if (pFireBunny->HasAura(SPELL_FLAMES_LARGE))
                        IsVictory = false;
            if (IsVictory)
            {
                DoScriptText(YELL_VICTORY, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_Q_STOP_THE_FIRE, CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_Q_LET_THE_FIRES_C, CAST_TRIGGERED);
                m_creature->ForcedDespawn(5000);
                uiPhase = PHASE_END;
                return;
            }
        }

        if (m_uiEventTimer < uiDiff)
        {
            switch(uiPhase)
            {
                case PHASE_1ST_SPEACH:
                    DoScriptText(YELL_1ST, m_creature);
                    m_uiEventTimer = 2 *MINUTE*IN_MILLISECONDS;
                    break;

                case PHASE_2ND_SPEACH:
                    DoScriptText(YELL_2ND, m_creature);
                    m_uiEventTimer = 0.5 *MINUTE*IN_MILLISECONDS;
                    break;
                case PHASE_FAIL:
                    DoScriptText(YELL_FAIL, m_creature);
                    m_creature->ForcedDespawn(10000);
                    for (GUIDList::iterator itr = lFireBunnies.begin(); itr != lFireBunnies.end(); ++itr)
                        if (Creature* pFireBunny = m_creature->GetMap()->GetCreature(*itr))
                        {
                            if (pFireBunny->HasAura(SPELL_FLAMES_LARGE))
                                pFireBunny->RemoveAurasDueToSpell(SPELL_FLAMES_LARGE);
                            pFireBunny->CastSpell(m_creature, SPELL_SMOKE, true);
                            pFireBunny->ForcedDespawn(60000);
                        }
                    break;
            }
            ++uiPhase;
            DoCastSpellIfCan(m_creature, SPELL_LAUGH_DELAYED_8);
        }
        else
            m_uiEventTimer -= uiDiff;

        if (m_uiConflagrationTimer < uiDiff)
        {
            bIsConflagrating = !bIsConflagrating;
            m_creature->GetMotionMaster()->MovementExpired();
            m_creature->GetMotionMaster()->MoveTargetedHome();
            m_uiConflagrationProcTimer = 2000;
            m_uiConflagrationTimer = bIsConflagrating ? 10000 : 30000;
            if (bIsConflagrating)
                DoScriptText(YELL_CONFLAGRATION, m_creature);
        }
        else
            m_uiConflagrationTimer -= uiDiff;

        if (bIsConflagrating)
            if (m_uiConflagrationProcTimer < uiDiff)
            {
                m_uiConflagrationProcTimer = 2000;
                if (lFireBunnies.empty())
                {
                    std::list<Creature*> tempFireBunnies;
                    GetCreatureListWithEntryInGrid(tempFireBunnies, m_creature, NPC_FIRE_BUNNY, 50.0f);
                    for (std::list<Creature*>::iterator itr = tempFireBunnies.begin(); itr != tempFireBunnies.end(); ++itr)
                        lFireBunnies.push_back((*itr)->GetObjectGuid());
                }

                if (lFireBunnies.empty())
                {
                    m_creature->ForcedDespawn(5000);
                    error_log("Missing DB spawns of Fire Bunnies (Horseman Village Event)");
                    uiPhase = PHASE_END;
                    return;
                }

                if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    return;
                
                for (GUIDList::iterator itr = lFireBunnies.begin(); itr != lFireBunnies.end(); ++itr)
                    if (Creature* pFireBunny = m_creature->GetMap()->GetCreature(*itr))
                        if (!pFireBunny->HasAura(SPELL_FLAMES_LARGE))
                        {
                            if (DoCastSpellIfCan(pFireBunny, SPELL_CONFLAGRATE) != CAST_OK)
                            {
                                float x,y,z;
                                pFireBunny->GetPosition(x,y,z);
                                pFireBunny->GetClosePoint(x,y,z,0,5,0);
                                m_creature->GetMotionMaster()->MovePoint(0, x,y,z+15);
                                break;
                            }
                        }
            }
            else
                m_uiConflagrationProcTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_shade_of_horseman(Creature* pCreature)
{
    return new npc_shade_of_horsemanAI (pCreature);
};

/*########
npc_wild_turkey
#########*/
enum
{
    EMOTE_TURKEY_HUNTER              = -1730001,
    EMOTE_TURKEY_DOMINATION          = -1730002,
    EMOTE_TURKEY_SLAUGHTER           = -1730003,
    EMOTE_TURKEY_TRIUMPH             = -1730004,

    SPELL_TURKEY_TRACKER             = 62014,
    SPELL_PH_KILL_COUNTER_VISUAL_MAX = 62021
};
struct MANGOS_DLL_DECL npc_wild_turkeyAI : public ScriptedAI
{
    npc_wild_turkeyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_creature->RemoveAllAuras();
    }

    void Reset(){}

    void UpdateAI(const uint32 uiDiff)
    {
        DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit* pKiller)
    {
        if (pKiller && pKiller->GetTypeId() == TYPEID_PLAYER)
        {
            pKiller->CastSpell(pKiller, SPELL_TURKEY_TRACKER, true);
            Aura * pAura = pKiller->GetAura(SPELL_TURKEY_TRACKER, EFFECT_INDEX_0);
            if (pAura)
            {
                uint32 stacks = pAura->GetStackAmount();
                switch (stacks)
                {
                    case 10:
                    {
                        DoScriptText(EMOTE_TURKEY_HUNTER, m_creature, pKiller);
                        break;
                    }
                    case 20:
                    {
                        DoScriptText(EMOTE_TURKEY_DOMINATION, m_creature, pKiller);
                        break;
                    }
                    case 30:
                    {
                        DoScriptText(EMOTE_TURKEY_SLAUGHTER, m_creature, pKiller);
                        break;
                    }
                    case 40:
                    {
                        DoScriptText(EMOTE_TURKEY_TRIUMPH, m_creature, pKiller);
                        pKiller->CastSpell(pKiller, SPELL_PH_KILL_COUNTER_VISUAL_MAX, true);
                        break;
                    }
                    default: break;
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_wild_turkey(Creature* pCreature)
{
    return new npc_wild_turkeyAI (pCreature);
};

/*###### 
## npc_experience 
######*/ 

#define EXP_COST                100000//10 00 00 copper (10golds)
#define GOSSIP_TEXT_EXP         14736
#define GOSSIP_XP_OFF           "I no longer wish to gain experience."
#define GOSSIP_XP_ON            "I wish to start gaining experience again."

bool GossipHello_npc_experience(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_XP_OFF, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_XP_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    pPlayer->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_EXP, pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_experience(Player* pPlayer, Creature* /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
{
    bool noXPGain = pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_XP_USER_DISABLED);
    bool doSwitch = false;

    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1://xp off
            {
                if (!noXPGain)//does gain xp
                    doSwitch = true;//switch to don't gain xp
            }
            break;
        case GOSSIP_ACTION_INFO_DEF + 2://xp on
            {
                if (noXPGain)//doesn't gain xp
                    doSwitch = true;//switch to gain xp
            }
            break;
    }
    if (doSwitch)
    {
        if (pPlayer->GetMoney() < EXP_COST)
            pPlayer->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
        else if (noXPGain)
        {
            pPlayer->ModifyMoney(-EXP_COST);
            pPlayer->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_XP_USER_DISABLED);
        }
        else if (!noXPGain)
        {
            pPlayer->ModifyMoney(-EXP_COST);
            pPlayer->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_XP_USER_DISABLED);
        }
    }
    pPlayer->PlayerTalkClass->CloseGossip();
    return true;
}

void AddSC_npcs_special()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_air_force_bots";
    pNewScript->GetAI = &GetAI_npc_air_force_bots;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_chicken_cluck";
    pNewScript->GetAI = &GetAI_npc_chicken_cluck;
    pNewScript->pQuestAcceptNPC =   &QuestAccept_npc_chicken_cluck;
    pNewScript->pQuestRewardedNPC = &QuestRewarded_npc_chicken_cluck;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_dancing_flames";
    pNewScript->GetAI = &GetAI_npc_dancing_flames;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_injured_patient";
    pNewScript->GetAI = &GetAI_npc_injured_patient;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_doctor";
    pNewScript->GetAI = &GetAI_npc_doctor;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_doctor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_garments_of_quests";
    pNewScript->GetAI = &GetAI_npc_garments_of_quests;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_guardian";
    pNewScript->GetAI = &GetAI_npc_guardian;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_innkeeper";
    pNewScript->pGossipHello = &GossipHello_npc_innkeeper;
    pNewScript->pGossipSelect = &GossipSelect_npc_innkeeper;
    pNewScript->RegisterSelf(false);                         // script and error report disabled, but script can be used for custom needs, adding ScriptName

    pNewScript = new Script;
    pNewScript->Name = "npc_mount_vendor";
    pNewScript->pGossipHello =  &GossipHello_npc_mount_vendor;
    pNewScript->pGossipSelect = &GossipSelect_npc_mount_vendor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_sayge";
    pNewScript->pGossipHello = &GossipHello_npc_sayge;
    pNewScript->pGossipSelect = &GossipSelect_npc_sayge;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_tabard_vendor";
    pNewScript->pGossipHello =  &GossipHello_npc_tabard_vendor;
    pNewScript->pGossipSelect = &GossipSelect_npc_tabard_vendor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_mirror_image";
    pNewScript->GetAI = &GetAI_npc_mirror_image;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_snake_trap_serpents";
    pNewScript->GetAI = &GetAI_npc_snake_trap_serpents;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_runeblade";
    pNewScript->GetAI = &GetAI_npc_rune_blade;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_death_knight_gargoyle";
    pNewScript->GetAI = &GetAI_npc_death_knight_gargoyle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_risen_ally";
    pNewScript->GetAI = &GetAI_npc_risen_ally;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_explosive_decoy";
    pNewScript->GetAI = &GetAI_npc_explosive_decoy;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_eye_of_kilrogg";
    pNewScript->GetAI = &GetAI_npc_eye_of_kilrogg;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_horseman_fire_bunny";
    pNewScript->GetAI = &GetAI_npc_horseman_fire_bunny;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_shade_of_horseman";
    pNewScript->GetAI = &GetAI_npc_shade_of_horseman;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_wild_turkey";
    pNewScript->GetAI = &GetAI_npc_wild_turkey;
    pNewScript->RegisterSelf();
    
    pNewScript = new Script;
    pNewScript->Name = "npc_experience";
    pNewScript->pGossipHello =  &GossipHello_npc_experience;
    pNewScript->pGossipSelect = &GossipSelect_npc_experience;
    pNewScript->RegisterSelf();
}
