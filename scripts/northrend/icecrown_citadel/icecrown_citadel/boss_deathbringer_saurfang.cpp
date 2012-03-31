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
SDName: boss_deathbringer_saurfang
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

// talks
enum
{
    SAY_AGGRO                   = -1631028,
    SAY_FALLENCHAMPION          = -1631029,
    SAY_BLOODBEASTS             = -1631030,
    SAY_SLAY_1                  = -1631031,
    SAY_SLAY_2                  = -1631032,
    SAY_BERSERK                 = -1631033,
    SAY_DEATH                   = -1631034,
    SAY_INTRO_ALLY_0            = -1631035,
    SAY_INTRO_ALLY_1            = -1631036,
    SAY_INTRO_ALLY_2            = -1631037,
    SAY_INTRO_ALLY_3            = -1631038,
    SAY_INTRO_ALLY_4            = -1631039,
    SAY_INTRO_ALLY_5            = -1631040,
    SAY_INTRO_HORDE_1           = -1631041,
    SAY_INTRO_HORDE_2           = -1631042,
    SAY_INTRO_HORDE_3           = -1631043,
    SAY_INTRO_HORDE_4           = -1631044,
    SAY_INTRO_HORDE_5           = -1631045,
    SAY_INTRO_HORDE_6           = -1631046,
    SAY_INTRO_HORDE_7           = -1631047,
    SAY_INTRO_HORDE_8           = -1631048,
    SAY_INTRO_HORDE_9           = -1631049,
    SAY_OUTRO_ALLY_1            = -1631050,
    SAY_OUTRO_ALLY_2            = -1631051,
    SAY_OUTRO_ALLY_3            = -1631052,
    SAY_OUTRO_ALLY_4            = -1631053,
    SAY_OUTRO_ALLY_5            = -1631054,
    SAY_OUTRO_ALLY_6            = -1631055,
    SAY_OUTRO_ALLY_7            = -1631056,
    SAY_OUTRO_ALLY_8            = -1631057,
    SAY_OUTRO_ALLY_9            = -1631058,
    SAY_OUTRO_ALLY_10           = -1631059,
    SAY_OUTRO_ALLY_11           = -1631060,
    SAY_OUTRO_ALLY_12           = -1631061,
    SAY_OUTRO_ALLY_13           = -1631062,
    SAY_OUTRO_ALLY_14           = -1631063,
    SAY_OUTRO_ALLY_15           = -1631064,
    SAY_OUTRO_ALLY_16           = -1631065,
    SAY_OUTRO_HORDE_1           = -1631066,
    SAY_OUTRO_HORDE_2           = -1631067,
    SAY_OUTRO_HORDE_3           = -1631068,
    SAY_OUTRO_HORDE_4           = -1631069,
};

enum
{
    // Intro
    SPELL_GRIP_OF_AGONY                     = 70572,

    // Blood Power
    SPELL_BLOOD_POWER                       = 72371,
    SPELL_BLOOD_LINK_BEAST                  = 72176, // proc aura for Blood Beasts
    SPELL_BLOOD_LINK                        = 72202, // cast on Saurfang to give 1 Blood Power

    // Mark of the Fallen Champion
    SPELL_MARK_OF_FALLEN_CHAMPION           = 72256, // proc on melee hit, dmg to marked targets
    SPELL_MARK_OF_FALLEN_CHAMPION_DEBUFF    = 72293, // proc on death - heal Saurfang
    SPELL_REMOVE_MARKS                      = 72257,

    // Rune of Blood
    SPELL_RUNE_OF_BLOOD                     = 72408, // cast on self on aggro
    SPELL_RUNE_OF_BLOOD_DEBUFF              = 72410,

    // Blood Nova
    SPELL_BLOOD_NOVA                        = 72378,

    // Boiling Blood
    SPELL_BOILING_BLOOD                     = 72385,

    // Blood Beasts
    SPELL_CALL_BLOOD_BEAST_1                = 72172,
    SPELL_CALL_BLOOD_BEAST_2                = 72173,
    SPELL_CALL_BLOOD_BEAST_3                = 72356,
    SPELL_CALL_BLOOD_BEAST_4                = 72357,
    SPELL_CALL_BLOOD_BEAST_5                = 72358,

    SPELL_SCENT_OF_BLOOD                    = 72769,
    SPELL_SCENT_OF_BLOOD_TRIGGERED          = 72771, // doesn't trigger, so cast in script...
    SPELL_RESISTANT_SKIN                    = 72723,

    // enrage
    SPELL_BERSERK                           = 26662,
    SPELL_FRENZY                            = 72737,

    //summons
    NPC_BLOOD_BEAST                         = 38508
};

enum Equipment
{
    EQUIP_MAIN           = 50798,
    EQUIP_OFFHAND        = EQUIP_NO_CHANGE,
    EQUIP_RANGED         = EQUIP_NO_CHANGE,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum SaurfangEvent
{
    NPC_OVERLORD_SAURFANG       = 37187,
    NPC_KORKRON_REAVER          = 37920,

    NPC_MURADIN_BRONZEBEARD     = 37200,
    NPC_ALLIANCE_MASON          = 37902,

    SPELL_VEHICLE_HARDCODED     = 46598, // Deathbringer enters Overlord
};

// positions
float fPositions[12][4] =
{
    {-468.05f, 2211.69f, 541.11f, 3.16f}, // Deathbringer teleport point
    {-491.30f, 2211.35f, 541.11f, 3.16f}, // Deathbringer dest point
    {-559.82f, 2211.29f, 539.30f, 6.28f}, // main npc summon/despawn point
    {-561.87f, 2215.94f, 539.30f, 6.28f}, // guard npc1 summon/despawn point
    {-560.17f, 2214.17f, 539.30f, 6.28f}, // guard npc2 summon/despawn point
    {-560.17f, 2207.71f, 539.30f, 6.28f}, // guard npc3 summon/despawn point
    {-561.87f, 2205.68f, 539.30f, 6.28f}, // guard npc4 summon/despawn point
    {-534.82f, 2211.29f, 539.30f, 6.28f}, // main npc first move
    {-536.87f, 2215.94f, 539.30f, 6.28f}, // guard npc1 first move
    {-535.17f, 2214.17f, 539.30f, 6.28f}, // guard npc2 first move
    {-535.17f, 2207.71f, 539.30f, 6.28f}, // guard npc3 first move
    {-536.87f, 2205.68f, 539.30f, 6.28f} // guard npc4 first move
};

// passive guards AI
struct MANGOS_DLL_DECL npc_deathbringer_event_guards_iccAI : public ScriptedAI
{
    npc_deathbringer_event_guards_iccAI(Creature* pCreature) : ScriptedAI(pCreature){}

    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}
    void AttackStart(Unit *pWho){}
};

// Event handler
struct MANGOS_DLL_DECL npc_highlord_saurfang_iccAI : public base_icc_bossAI
{
    npc_highlord_saurfang_iccAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        m_bIsEventStarted = false;
        m_uiEventStep = 0;
        m_uiNextEventTimer = 0;

        m_lGuards.clear();
    }

    uint32 m_uiNextEventTimer;
    uint32 m_uiEventStep;
    bool m_bIsEventStarted;

    GUIDList m_lGuards;

    void Reset(){}

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        uiDamage = 0;
    }

    void AttackStart(Unit *pWho){}

    Creature* GetSaurfang()
    {
        if (m_pInstance)
            return m_pInstance->GetSingleCreatureFromStorage(NPC_DEATHBRINGER_SAURFANG);

        return NULL;
    }

    void DoSummonGuards()
    {
        // 4 adds
        if (Creature *pTmp = m_creature->SummonCreature(NPC_KORKRON_REAVER, fPositions[3][0], fPositions[3][1], fPositions[3][2], fPositions[3][3], TEMPSUMMON_MANUAL_DESPAWN, 0))
            m_lGuards.push_back(pTmp->GetObjectGuid());
        if (Creature *pTmp = m_creature->SummonCreature(NPC_KORKRON_REAVER, fPositions[4][0], fPositions[4][1], fPositions[4][2], fPositions[4][3], TEMPSUMMON_MANUAL_DESPAWN, 0))
            m_lGuards.push_back(pTmp->GetObjectGuid());
        if (Creature *pTmp = m_creature->SummonCreature(NPC_KORKRON_REAVER, fPositions[5][0], fPositions[5][1], fPositions[5][2], fPositions[5][3], TEMPSUMMON_MANUAL_DESPAWN, 0))
            m_lGuards.push_back(pTmp->GetObjectGuid());
        if (Creature *pTmp = m_creature->SummonCreature(NPC_KORKRON_REAVER, fPositions[6][0], fPositions[6][1], fPositions[6][2], fPositions[6][3], TEMPSUMMON_MANUAL_DESPAWN, 0))
            m_lGuards.push_back(pTmp->GetObjectGuid());
    }

    void DoStartEvent()
    {
        m_bIsEventStarted = true;
        NextStep(0);
    }

    void DoContinueEvent()
    {
        NextStep();
    }

    void NextStep(uint32 uiTime = 1000)
    {
        ++m_uiEventStep;
        m_uiNextEventTimer = uiTime;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsEventStarted || !m_pInstance)
            return;

        if (m_uiNextEventTimer <= uiDiff)
        {
            switch (m_uiEventStep)
            {
                case 0:
                {
                    break;
                }
                case 1:
                {
                    DoSummonGuards();
                    m_pInstance->DoUseDoorOrButton(GO_SAURFANG_DOOR, 0, false);
                    if (Creature *pSaurfang = GetSaurfang())
                        pSaurfang->GetMotionMaster()->MovePoint(0, fPositions[1][0], fPositions[1][1], fPositions[1][2]);
                    NextStep(1000);
                    break;
                }
                case 2:
                {
                    m_pInstance->DoUseDoorOrButton(GO_SAURFANG_DOOR, 0, true);
                    DoScriptText(SAY_INTRO_HORDE_1, m_creature);
                    m_creature->GetMotionMaster()->MovePoint(0, fPositions[7][0], fPositions[7][1], fPositions[7][2]);

                    // move guards
                    int8 n = 8;
                    for (GUIDList::iterator i = m_lGuards.begin(); i != m_lGuards.end(); ++i)
                    {
                        if (Creature *pTmp = m_creature->GetMap()->GetCreature(*i))
                            pTmp->GetMotionMaster()->MovePoint(0, fPositions[n][0], fPositions[n][1], fPositions[n][2]);
                        ++n;
                    }
                    NextStep(6000);
                    break;
                }
                case 3:
                {
                    if (Creature *pSaurfang = GetSaurfang())
                        DoScriptText(SAY_INTRO_HORDE_2, pSaurfang);
                    NextStep(13000);
                    break;
                }
                case 4:
                {
                    DoScriptText(SAY_INTRO_HORDE_3, m_creature);
                    NextStep(7000);
                    break;
                }
                case 5:
                {
                    if (Creature *pSaurfang = GetSaurfang())
                        DoScriptText(SAY_INTRO_HORDE_4, pSaurfang);
                    NextStep(11000);
                    break;
                }
                case 6:
                {
                    DoScriptText(SAY_INTRO_HORDE_5, m_creature);
                    NextStep(16000);
                    break;
                }
                case 7:
                {
                    DoScriptText(SAY_INTRO_HORDE_6, m_creature);
                    NextStep(15000);
                    break;
                }
                case 8:
                {
                    DoScriptText(SAY_INTRO_HORDE_7, m_creature);
                    NextStep(3000);
                    break;
                }
                case 9:
                {
                    float x, y, z;
                    if (Creature *pSaurfang = GetSaurfang())
                        pSaurfang->GetPosition(x, y, z);

                    DoScriptText(SAY_INTRO_HORDE_8, m_creature);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                    m_creature->SetWalk(false);
                    m_creature->GetMotionMaster()->MovePoint(0, x, y, z);

                    // move guards
                    for (GUIDList::iterator i = m_lGuards.begin(); i != m_lGuards.end(); ++i)
                    {
                        if (Creature *pTmp = m_creature->GetMap()->GetCreature(*i))
                        {
                            pTmp->SetWalk(false);
                            pTmp->GetMotionMaster()->MovePoint(0, x, y, z);
                            pTmp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                            pTmp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                        }
                    }

                    if (Creature *pSaurfang = GetSaurfang())
                        pSaurfang->CastSpell(pSaurfang, SPELL_GRIP_OF_AGONY, true);

                    NextStep(1500);
                    break;
                }
                case 10:
                {
                    float x, y, z;
                    // npcs float in air
                    m_creature->SetWalk(true);
                    m_creature->SetSpeedRate(MOVE_WALK, 3.0f);
                    m_creature->SetLevitate(true);
                    m_creature->GetPosition(x, y, z);
                    m_creature->GetMotionMaster()->MovePoint(0, x, y, z + frand(5.0f, 7.0f));

                    // move guards
                    for (GUIDList::iterator i = m_lGuards.begin(); i != m_lGuards.end(); ++i)
                    {
                        if (Creature *pTmp = m_creature->GetMap()->GetCreature(*i))
                        {
                            pTmp->SetWalk(true);
                            pTmp->SetSpeedRate(MOVE_WALK, 3.0f);
                            pTmp->SetLevitate(true);
                            pTmp->GetPosition(x, y, z);
                            pTmp->GetMotionMaster()->MovePoint(0, x, y, z + frand(5.0f, 7.0f));
                        }
                    }
                    if (Creature *pSaurfang = GetSaurfang())
                        DoScriptText(SAY_INTRO_HORDE_9, pSaurfang);

                    NextStep(10000);
                    break;
                }
                case 11:
                {
                    if (Creature *pSaurfang = GetSaurfang())
                    {
                        pSaurfang->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pSaurfang->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pSaurfang->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        pSaurfang->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                        pSaurfang->SetInCombatWithZone();
                    }

                    NextStep();
                    break;
                }
                case 12:
                {
                    // fight is in progress
                    break;
                }
                case 13:
                {
                    float x, y, z;

                    m_creature->HandleEmote(EMOTE_ONESHOT_NONE);
                    m_creature->GetPosition(x, y, z);
                    m_creature->UpdateAllowedPositionZ(x, y, z);
                    m_creature->GetMotionMaster()->MovePoint(0, x, y, z);

                    // move guards
                    for (GUIDList::iterator i = m_lGuards.begin(); i != m_lGuards.end(); ++i)
                    {
                        if (Creature *pTmp = m_creature->GetMap()->GetCreature(*i))
                        {
                            pTmp->GetPosition(x, y, z);
                            m_creature->UpdateAllowedPositionZ(x, y, z);
                            pTmp->GetMotionMaster()->MovePoint(0, x, y, z);
                            pTmp->HandleEmote(EMOTE_ONESHOT_NONE);
                        }
                    }
                    NextStep(1000);
                    break;
                }
                case 14:
                {
                    m_creature->SetLevitate(false);
                    m_creature->SetSpeedRate(MOVE_WALK, 1.0f);
                    m_creature->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY);

                    for (GUIDList::iterator i = m_lGuards.begin(); i != m_lGuards.end(); ++i)
                    {
                        if (Creature *pGuard = m_creature->GetMap()->GetCreature(*i))
                        {
                            pGuard->SetLevitate(false);
                            pGuard->SetSpeedRate(MOVE_WALK, 1.0f);
                            pGuard->GetMotionMaster()->MoveFollow(m_creature, frand(2.0f, 5.0f), frand(M_PI_F / 2, 1.5f * M_PI_F));
                            pGuard->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY);
                        }
                    }

                    NextStep(1000);
                    break;
                }
                case 15:
                {
                    DoScriptText(SAY_OUTRO_HORDE_1, m_creature);
                    m_creature->HandleEmote(EMOTE_ONESHOT_KNEEL);
                    NextStep(3000);
                    break;
                }
                case 16:
                {
                    if (Creature *pSaurfang = GetSaurfang())
                    {
                        float x, y, z;
                        pSaurfang->GetContactPoint(m_creature, x, y, z, 1.0f);
                        m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
                    }
                    NextStep(0);
                    break;
                }
                case 17:
                {
                    if (Creature *pSaurfang = GetSaurfang())
                    {
                        // wait until Overlord comes to Deathbringer
                        if (!m_creature->IsWithinDist(pSaurfang, 2.0f))
                            return;

                        m_creature->StopMoving();
                        m_creature->SetFacingToObject(pSaurfang);
                    }
                    NextStep(1000);
                    break;
                }
                case 18:
                {
                    if (Creature *pSaurfang = GetSaurfang())
                    {
                        DoScriptText(SAY_OUTRO_HORDE_2, m_creature);
                        m_creature->HandleEmote(EMOTE_ONESHOT_KNEEL);
                        // doesnt work, invalid target (probably related to strangluating aura problem, maybe factions?)
                        pSaurfang->CastSpell(m_creature, SPELL_VEHICLE_HARDCODED, true);
                    }
                    NextStep(3000);
                    break;
                }
                case 19:
                {
                    // take deathbringer
                    DoScriptText(SAY_OUTRO_HORDE_3, m_creature);
                    NextStep(11000);
                    break;
                }
                case 20:
                {
                    DoScriptText(SAY_OUTRO_HORDE_4, m_creature);
                    m_creature->GetMotionMaster()->MovePoint(0, fPositions[2][0], fPositions[2][1], fPositions[2][2]);
                    NextStep();
                    break;
                }
                case 21:
                {
                    // wait until coming to gunship boarding place
                    if (!m_creature->IsWithinDist2d(fPositions[2][0], fPositions[2][1], 3.0f))
                        return;

                    for (GUIDList::iterator i = m_lGuards.begin(); i != m_lGuards.end(); ++i)
                    {
                        if (Creature *pGuard = m_creature->GetMap()->GetCreature(*i))
                            pGuard->ForcedDespawn();
                    }
                    m_creature->ForcedDespawn();
                    return;
                }
                default:
                    break;
            }
        }
        else
            m_uiNextEventTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_deathbringer_saurfangAI : public base_icc_bossAI
{
    boss_deathbringer_saurfangAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        m_powerBloodPower = m_creature->getPowerType(); // don't call this function multiple times in script
        m_bTeleported = false;
        m_bIsIntroStarted = false;
        m_guidEventNpcGuid.Clear();
        Reset();
    }

    uint32 m_uiRuneOfBloodTimer;
    uint32 m_uiBoilingBloodTimer;
    uint32 m_uiBloodNovaTimer;
    uint32 m_uiBloodBeastsTimer;
    uint32 m_uiScentOfBloodTimer;
    uint32 m_uiBerserkTimer;

    bool m_bIsFrenzied;

    bool m_bTeleported;
    bool m_bIsIntroStarted;
    bool m_bIsAlliance;

    Powers m_powerBloodPower;

    ObjectGuid m_guidEventNpcGuid;

    void Reset()
    {
        m_uiRuneOfBloodTimer    = 20000;
        m_uiBoilingBloodTimer   = urand(10000, 35000);
        m_uiBloodNovaTimer      = urand(16000, 35000);
        m_uiBloodBeastsTimer    = 40000;
        m_uiScentOfBloodTimer   = 47000; // 5 seconds after beasts engage in combat
        m_uiBerserkTimer        = (m_bIsHeroic ? 6 : 8) * MINUTE * IN_MILLISECONDS;

        m_bIsFrenzied = false;

        m_creature->SetPower(m_powerBloodPower, 0);
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (!m_bTeleported && pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster())
        {
            // teleport behind door
            m_creature->NearTeleportTo(fPositions[0][0], fPositions[0][1], fPositions[0][2], fPositions[0][3]);
            m_bTeleported = true;
        }

        if (m_bTeleported && !m_bIsIntroStarted && pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster() && m_creature->GetDistance2d(pWho) < 50.0f)
        {
            m_bIsAlliance = false; //((Player*)pWho)->GetTeam() == ALLIANCE;
            DoSummonEventNpc();
            m_bIsIntroStarted = true;
        }
    }

    void DoSummonEventNpc()
    {
        // main npc: Saurfang or Muradin
        if (Creature *pTmp = m_creature->SummonCreature(m_bIsAlliance ? NPC_MURADIN_BRONZEBEARD : NPC_OVERLORD_SAURFANG, fPositions[2][0], fPositions[2][1], fPositions[2][2], fPositions[2][3], TEMPSUMMON_MANUAL_DESPAWN, 0))
        {
            m_guidEventNpcGuid = pTmp->GetObjectGuid();
            if (npc_highlord_saurfang_iccAI *pTmpAI = dynamic_cast<npc_highlord_saurfang_iccAI*>(pTmp->AI()))
                pTmpAI->DoStartEvent();
        }
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);

        DoCastSpellIfCan(m_creature, SPELL_BLOOD_POWER, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_RUNE_OF_BLOOD, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_MARK_OF_FALLEN_CHAMPION, CAST_TRIGGERED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, FAIL);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
    }

    // used for unlocking bugged encounter
    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, DONE);

        DoScriptText(SAY_DEATH, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_REMOVE_MARKS, CAST_TRIGGERED);

        if (Creature *pTmp = m_creature->GetMap()->GetCreature(m_guidEventNpcGuid))
        {
            if (npc_highlord_saurfang_iccAI *pTmpAI = dynamic_cast<npc_highlord_saurfang_iccAI*>(pTmp->AI()))
                pTmpAI->DoContinueEvent();
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_SLAY_1 - urand(0,1), m_creature, pVictim);
    }

    void JustSummoned(Creature *pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_BLOOD_BEAST)
        {
            pSummoned->CastSpell(pSummoned, SPELL_BLOOD_LINK_BEAST, true);
            pSummoned->CastSpell(pSummoned, SPELL_RESISTANT_SKIN, true);
            pSummoned->setFaction(m_creature->getFaction());
        }
    }

    Player* SelectRandomPlayerForMark()
    {
        Player *pResult = NULL;
        GUIDList lPlayers;
        ThreatList const &threatlist = m_creature->getThreatManager().getThreatList();

        if (!threatlist.empty())
        {
            for (ThreatList::const_iterator itr = threatlist.begin();itr != threatlist.end(); ++itr)
            {
                ObjectGuid const &guid = (*itr)->getUnitGuid();
                if (guid.IsPlayer() && guid != m_creature->getVictim()->GetObjectGuid()) // exclude current target
                {
                    // check if player already has the mark
                    if (Player *pPlayer = m_creature->GetMap()->GetPlayer(guid))
                    {
                        if (!pPlayer->HasAura(SPELL_MARK_OF_FALLEN_CHAMPION_DEBUFF))
                            lPlayers.push_back(guid);
                    }
                }
            }
        }

        if (!lPlayers.empty())
        {
            GUIDList::iterator i = lPlayers.begin();
            uint32 max = uint32(lPlayers.size() - 1);

            if (max > 0)
                std::advance(i, urand(0, max));

            pResult = m_creature->GetMap()->GetPlayer(*i);
        }

        // last option - current target
        if (!pResult)
        {
            Unit *pVictim = m_creature->getVictim();
            if (pVictim && pVictim->GetTypeId() == TYPEID_PLAYER && !pVictim->HasAura(SPELL_MARK_OF_FALLEN_CHAMPION_DEBUFF))
                pResult = (Player*)pVictim;
        }

        return pResult;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Mark of the Fallen Champion
        if (m_creature->GetPower(m_powerBloodPower) >= 100)
        {
            if (Player *pTarget = SelectRandomPlayerForMark())
            {
                if (DoCastSpellIfCan(pTarget, SPELL_MARK_OF_FALLEN_CHAMPION_DEBUFF) == CAST_OK)
                {
                    m_creature->SetPower(m_powerBloodPower, 0); // reset Blood Power
                    // decrease the buff
                    m_creature->RemoveAurasDueToSpell(72371);
                    int32 power = m_creature->GetPower(m_powerBloodPower);
                    m_creature->CastCustomSpell(m_creature, 72371, &power, &power, NULL, true);
                    DoScriptText(SAY_FALLENCHAMPION, m_creature);
                }
            }
        }

        // Frenzy (soft enrage)
        if (!m_bIsFrenzied)
        {
            if (m_creature->GetHealthPercent() <= 30.0f)
            {
                DoCastSpellIfCan(m_creature, SPELL_FRENZY, CAST_TRIGGERED);
                DoScriptText(SAY_BERSERK, m_creature);
                m_bIsFrenzied = true;
            }
        }

        // Berserk (hard enrage)
        if (m_uiBerserkTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiBerserkTimer = (m_bIsHeroic ? 6 : 8) * MINUTE * IN_MILLISECONDS;
            }
        }
        else
            m_uiBerserkTimer -= uiDiff;

        // Rune of Blood
        if (m_uiRuneOfBloodTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD_DEBUFF) == CAST_OK)
                m_uiRuneOfBloodTimer = 20000;
        }
        else
            m_uiRuneOfBloodTimer -= uiDiff;

        // Boiling Blood
        if (m_uiBoilingBloodTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BOILING_BLOOD) == CAST_OK)
                m_uiBoilingBloodTimer = urand(10000, 35000);
        }
        else
            m_uiBoilingBloodTimer -= uiDiff;

        // Boiling Blood
        if (m_uiBloodNovaTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BLOOD_NOVA) == CAST_OK)
                m_uiBloodNovaTimer = urand(16000, 35000);
        }
        else
            m_uiBloodNovaTimer -= uiDiff;

        // Call Blood Beasts
        if (m_uiBloodBeastsTimer <= uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_1, true);
            m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_2, true);

            if (m_bIs25Man)
            {
                m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_3, true);
                m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_4, true);
                m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_5, true);
            }

            m_uiBloodBeastsTimer  = 40000;

            if (m_bIsHeroic)
                m_uiScentOfBloodTimer = 7000; // 5 seconds after beasts engage in combat

            DoScriptText(SAY_BLOODBEASTS, m_creature);
        }
        else
            m_uiBloodBeastsTimer -= uiDiff;

        // Scent of Blood
        if (m_bIsHeroic)
        {
            if (m_uiScentOfBloodTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SCENT_OF_BLOOD, CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_SCENT_OF_BLOOD_TRIGGERED, CAST_TRIGGERED);
                m_uiScentOfBloodTimer = 40000;
                // via DB
                m_creature->MonsterTextEmote("Deathbringer Saurfang's Blood Beasts gain the scent of blood!", m_creature->getVictim(), true);
            }
            else
                m_uiScentOfBloodTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_deathbringer_saurfang(Creature* pCreature)
{
    return new boss_deathbringer_saurfangAI(pCreature);
}

CreatureAI* GetAI_npc_highlord_saurfang_icc(Creature* pCreature)
{
    return new npc_highlord_saurfang_iccAI(pCreature);
}

CreatureAI* GetAI_npc_deathbringer_event_guards_iccAI(Creature* pCreature)
{
    return new npc_deathbringer_event_guards_iccAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_blood_beastAI : public ScriptedAI
{
    mob_blood_beastAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiScentOfBloodTimer;
    uint32 m_uiReadyTimer;

    bool m_bIsReady;

    void Reset()
    {
        m_uiScentOfBloodTimer = 5000;

        m_uiReadyTimer = 2000;
        m_bIsReady = false;

        SetCombatMovement(false);
        m_creature->SetWalk(true);
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsReady)
        {
            if (m_uiReadyTimer <= uiDiff)
            {
                m_bIsReady = true;
                SetCombatMovement(true);
                m_creature->SetInCombatWithZone();
                if (m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            else
                m_uiReadyTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blood_beast(Creature* pCreature)
{
    return new mob_blood_beastAI(pCreature);
}

void AddSC_boss_deathbringer_saurfang()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_deathbringer_saurfang";
    newscript->GetAI = &GetAI_boss_deathbringer_saurfang;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_highlord_saurfang_icc";
    newscript->GetAI = &GetAI_npc_highlord_saurfang_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_deathbringer_event_guards_iccAI";
    newscript->GetAI = &GetAI_npc_deathbringer_event_guards_iccAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_blood_beast";
    newscript->GetAI = &GetAI_mob_blood_beast;
    newscript->RegisterSelf();
}
