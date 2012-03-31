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
SDName: boss_blood_queen_lanathel
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    // all phases
    SPELL_BERSERK                       = 26662,
    SPELL_SHROUD_OF_SORROW              = 70986,

    // phase ground
    SPELL_BLOOD_MIRROR                  = 70445,
 // SPELL_DELIRIOUS_SLASH               = 72261,
    SPELL_DELIRIOUS_SLASH_1             = 71623, // effect
    SPELL_DELIRIOUS_SLASH_2             = 72264, // with charge effect. cast on random target if offtank is not present?
    SPELL_SWARMING_SHADOWS              = 71861,
    SPELL_SWARMING_SHADOWS_TRIGGERED    = 71264, // triggered by previous
    SPELL_SWARMING_SHADOWS_AURA         = 71267,
    SPELL_PACT_OF_THE_DARKFALLEN        = 71336,
 // SPELL_VAMPIRIC_BITE                 = 71837,
    SPELL_VAMPIRIC_BITE_TRIGGERED       = 71726, // triggered spell with effects
 // SPELL_VAMPIRIC_BITE_PLAYER          = 70946, // used by players
    SPELL_TWILIGHT_BLOODBOLT            = 71445,
 // SPELL_TWILIGHT_BLOODBOLT_VISUAL     = 72313, // dummy effect
 // SPELL_TWILIGHT_BLOODBOLT_TRIGGERED1 = 71446, // one of the triggered spells
 // SPELL_TWILIGHT_BLOODBOLT_TRIGGERED2 = 71818, // another of the triggered spells. same effects but other spell id...
 // SPELL_PRESENCE_OF_DARKFALLEN        = 71952, // on heroic

    // phase air
    SPELL_INCITE_HORROR                 = 73070,
    SPELL_BLOODBOLT_WHIRL               = 71772,

    // others
    NPC_SWARMING_SHADOWS                = 38163,

    THIRST_QUENCHED_AURA                    = 72154
};

// talks
enum
{
    SAY_AGGRO                   = -1631121,
    SAY_BITE_1                  = -1631122,
    SAY_BITE_2                  = -1631123,
    SAY_SHADOWS                 = -1631124,
    SAY_PACT                    = -1631125,
    SAY_MC                      = -1631126,
    SAY_AIR_PHASE               = -1631127,
    SAY_BERSERK                 = -1631128,
    SAY_DEATH                   = -1631129,
};

static Locations QueenLocs[]=
{
    {4595.640137f, 2769.195557f, 400.137054f},  // 0 Phased
    {4595.904785f, 2769.315918f, 421.838623f},  // 1 Fly
};

#define PHASE_GROUND 1
#define PHASE_RUNNING 2
#define PHASE_AIR 3
#define PHASE_FLYING 4

#define POINT_CENTER_GROUND 1
#define POINT_CENTER_AIR 2

/**
 * Queen Lana'thel
 */
struct MANGOS_DLL_DECL boss_blood_queen_lanathelAI : public base_icc_bossAI
{
    boss_blood_queen_lanathelAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiPhase;

    uint32 m_uiBloodMirrorTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiPhaseTimer;
    uint32 m_uiVampiricBiteTimer;
    uint32 m_uiBloodboltTimer;
    uint32 m_uiPactTimer;
    uint32 m_uiSwarmingShadowsTimer;
    uint32 m_uiDeliriousSlashTimer;

    bool m_bHasBitten;

    void Reset()
    {
        m_uiPhase               = PHASE_GROUND;

        m_bHasBitten            = false; // for Vampiric Bite

        m_uiPhaseTimer          = 2 * MINUTE * IN_MILLISECONDS;
        m_uiEnrageTimer         = (5 * MINUTE + 30) * IN_MILLISECONDS;
        m_uiBloodMirrorTimer    = 0;
        m_uiDeliriousSlashTimer = 20000;
        m_uiVampiricBiteTimer   = 15000;
        m_uiBloodboltTimer      = urand(15000, 20000);
        m_uiPactTimer           = urand(20000, 25000);
        m_uiSwarmingShadowsTimer= urand(30000, 35000);
    }

    void JustReachedHome()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_LANATHEL, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        // entry missing in sd2 database
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            m_creature->MonsterYell("Is that all you got?", 0);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance) 
            m_pInstance->SetData(TYPE_LANATHEL, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_SHROUD_OF_SORROW, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLOOD_MIRROR, CAST_TRIGGERED);
    }

    void JustDied(Unit *pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_LANATHEL, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (uiData == POINT_CENTER_GROUND)
        {
            if (m_uiPhase == PHASE_RUNNING)
            {
                m_creature->GetMotionMaster()->Clear();
                m_uiPhase = PHASE_AIR; // start counting timer for Bloodbolt Whirl immediately

                if (DoCastSpellIfCan(m_creature, SPELL_INCITE_HORROR) == CAST_OK)
                {
                    // fly up
                    m_creature->SetWalk(true);
                    m_creature->SetLevitate(true);
                    m_creature->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_UNK_2);

                    m_creature->GetMotionMaster()->MovePoint(POINT_CENTER_AIR, QueenLocs[1].x, QueenLocs[1].y, QueenLocs[1].z, false);
                }
            }
            else
            {
                m_uiPhase = PHASE_GROUND;

                // stop flying
                m_creature->SetWalk(false);
                m_creature->SetLevitate(false);
                m_creature->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_UNK_2);

                SetCombatMovement(true);
                if (m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
        }
        else if (uiData == POINT_CENTER_AIR)
        {
            // Bloodbolt Whirl
            if (DoCastSpellIfCan(m_creature, SPELL_BLOODBOLT_WHIRL) == CAST_OK)
            {
                m_uiPhase = PHASE_AIR;
                m_uiPhaseTimer = 7000;
                DoScriptText(SAY_AIR_PHASE, m_creature);
            }
        }
    }

    Unit* SelectClosestFriendlyTarget(Unit *pVictim)
    {
        Unit *pResult = NULL;

        if (m_pInstance)
        {
            float lastDist = 500.0f;
            const Map::PlayerList &players = m_pInstance->instance->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (!(*itr).getSource()->IsInWorld() ||                              // don't target not in world players
                    !(*itr).getSource()->isAlive() ||                                // don't target dead players
                    (*itr).getSource()->isGameMaster() ||                            // don't target GMs
                    (*itr).getSource()->GetObjectGuid() == pVictim->GetObjectGuid()) // don't target current victim
                    continue;

                float dist = pVictim->GetDistance((*itr).getSource());
                if (dist < lastDist)
                {
                    pResult = (*itr).getSource();
                    lastDist = dist;
                }
            }
        }

        return pResult;
    }

    Unit* SelectVampiricBiteTarget()
    {
        const ThreatList &threatList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator itr = threatList.begin();
        std::advance(itr, 1); // don't target the main tank
        for (;itr != threatList.end(); ++itr)
        {
            if (Unit *pVictim = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                if (!pVictim->HasAuraOfDifficulty(70867) && // Essence of the Blood Queen
                    !pVictim->HasAuraOfDifficulty(70877) && // Frenzied Bloodthirst
                    !pVictim->HasAuraOfDifficulty(70445) && // Blood Mirror
                    !pVictim->HasAuraOfDifficulty(70923))   // Uncontrollable Frenzy
                {
                    return pVictim;
                }
            }
        }

        return NULL;
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Enrage
        if (m_uiEnrageTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiEnrageTimer = (5 * MINUTE + 30) * IN_MILLISECONDS;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        switch (m_uiPhase)
        {
            case PHASE_GROUND:
            {
                // phase change timer
                if (m_uiPhaseTimer <= uiDiff)
                {
                    m_uiPhase = PHASE_RUNNING;
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->MovePoint(POINT_CENTER_GROUND, QueenLocs[0].x, QueenLocs[0].y, QueenLocs[0].z);
                    m_uiPhaseTimer = 13000;
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                // Blood Mirror
                if (m_uiBloodMirrorTimer <= uiDiff)
                {
                    if (Unit *pVictim = SelectClosestFriendlyTarget(m_creature->getVictim()))
                    {
                        pVictim->CastSpell(m_creature->getVictim(), SPELL_BLOOD_MIRROR, true, 0, 0, pVictim->GetObjectGuid());
                        m_uiBloodMirrorTimer = 5000;
                    }
                }
                else
                    m_uiBloodMirrorTimer -= uiDiff;

                // Delirious Slash
                if (m_bIsHeroic)
                {
                    if (m_uiDeliriousSlashTimer <= uiDiff)
                    {
                        /**
                         * Spell that handles targeting - we can do this here.
                         * if (DoCastSpellIfCan(m_creature, SPELL_DELIRIOUS_SLASH) == CAST_OK)
                         */
                        if (Unit *pTarget = SelectClosestFriendlyTarget(m_creature->getVictim()))
                        {
                            uint32 spell = SPELL_DELIRIOUS_SLASH_1;

                            // if target is not in 5yd range then cast spell with charge effect
                            if (!m_creature->IsWithinDist(pTarget, 5.0f))
                                spell = SPELL_DELIRIOUS_SLASH_2;

                            if (DoCastSpellIfCan(pTarget, spell) == CAST_OK)
                                m_uiDeliriousSlashTimer = 15000;
                        }
                    }
                    else
                        m_uiDeliriousSlashTimer -= uiDiff;
                }

                // Vampiric Bite
                if (!m_bHasBitten)
                {
                    if (m_uiVampiricBiteTimer <= uiDiff)
                    {
                        /**
                         * Spell handles targeting, but we can do this here.
                         * if (DoCastSpellIfCan(m_creature, SPELL_VAMPIRIC_BITE) == CAST_OK)
                         */
                        if (Unit *pTarget = SelectVampiricBiteTarget())
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_VAMPIRIC_BITE_TRIGGERED) == CAST_OK)
                            {
                                m_bHasBitten = true;
                                DoScriptText(SAY_BITE_1 - urand(0, 1), m_creature);
                            }
                        }
                    }
                    else
                        m_uiVampiricBiteTimer -= uiDiff;
                }

                // Twilight Bloodbolt
                if (m_uiBloodboltTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_TWILIGHT_BLOODBOLT) == CAST_OK)
                        m_uiBloodboltTimer = urand(15000, 20000);
                }
                else
                    m_uiBloodboltTimer -= uiDiff;

                // Pact of the Darkfallen
                if (m_uiPactTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_PACT_OF_THE_DARKFALLEN) == CAST_OK)
                    {
                        m_uiPactTimer = urand(20000, 25000);
                        DoScriptText(SAY_PACT, m_creature);
                    }
                }
                else
                    m_uiPactTimer -= uiDiff;

                // Swarming Shadows
                if (m_uiSwarmingShadowsTimer <= uiDiff)
                {
                    /**
                     * spell which handles picking targets
                     * but we can use SelectAttackingTarget() here
                     * if (DoCastSpellIfCan(m_creature, SPELL_SWARMING_SHADOWS) == CAST_OK)
                     */
                    if (Unit *pTarget = SelectRandomRangedTarget(m_creature))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_SWARMING_SHADOWS_TRIGGERED) == CAST_OK)
                        {
                            m_uiSwarmingShadowsTimer = urand(30000, 35000);
                            DoScriptText(SAY_SHADOWS, m_creature);
                        }
                    }
                }
                else
                    m_uiSwarmingShadowsTimer -= uiDiff;

                DoMeleeAttackIfReady();

                break;
            }
            case PHASE_RUNNING:
            case PHASE_FLYING:
            {
                // wait for arriving at the point
                break;
            }
            case PHASE_AIR:
            {
                // phase change timer
                if (m_uiPhaseTimer <= uiDiff)
                {
                    m_uiPhase = PHASE_FLYING;
                    m_uiPhaseTimer = 2 * MINUTE * IN_MILLISECONDS - 13000; // substract the air phase duration
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MovePoint(POINT_CENTER_GROUND, QueenLocs[0].x, QueenLocs[0].y, QueenLocs[0].z);
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                break;
            }
        }
    }
};

CreatureAI* GetAI_boss_blood_queen_lanathel(Creature* pCreature)
{
    return new boss_blood_queen_lanathelAI(pCreature);
}

/**
 * Swarming Shadows
 */
struct MANGOS_DLL_DECL mob_swarming_shadowsAI : public ScriptedAI
{
    mob_swarming_shadowsAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_creature->SetDisplayId(11686);
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
        DoCastSpellIfCan(m_creature, SPELL_SWARMING_SHADOWS_AURA, CAST_TRIGGERED);
    }

    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_swarming_shadows(Creature* pCreature)
{
    return new mob_swarming_shadowsAI(pCreature);
}

void AddSC_boss_blood_queen_lanathel()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_swarming_shadows";
    newscript->GetAI = &GetAI_mob_swarming_shadows;
    newscript->RegisterSelf();
}
