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
SDName: boss_professor_putricide
SD%Complete: 85%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"
#include "Vehicle.h"

enum BossSpells
{
    SPELL_BERSERK                   = 47008,

    // controlled abomination
    SPELL_MUTATED_TRANSFORMATION    = 70311,
    SPELL_EAT_OOZE                  = 72527,
    SPELL_REGURGITATED_OOZE         = 70539,
    SPELL_MUTATED_SLASH             = 70542,
    SPELL_MUTATED_AURA              = 70405,
    SPELL_ABOMINATION_POWER_DRAIN   = 70385, // prevents normal regen of abomination's power

    SPELL_UNSTABLE_EXPERIMENT       = 70351,
    SPELL_VOLATILE_EXPERIMENT       = 72842, // heroic
    SPELL_GREEN_OOZE_SUMMON         = 71412,
    SPELL_ORANGE_OOZE_SUMMON        = 71415,

    SPELL_OOZE_ADHESIVE             = 70447,
    SPELL_OOZE_ERUPTION             = 70492,

    SPELL_GASEOUS_BLOAT             = 70672,
    SPELL_EXPUNGED_GAS              = 70701,
    SPELL_GASEOUS_BLOAT_VISUAL      = 70215,

    SPELL_SLIME_PUDDLE              = 70341,
    SPELL_SLIME_PUDDLE_SUMMON       = 70342,
    SPELL_SLIME_PUDDLE_AURA         = 70343,
 // SPELL_SLIME_PUDDLE_TRIGGER      = 71424, // trigger summon spell from target?
 // SPELL_SLIME_PUDDLE_SUMMON_TRIG  = 71425,
    SPELL_GROW_STACKER              = 70345,
    SPELL_GROW_STACKER_GROW_AURA    = 70347,

    SPELL_MALLEABLE_GOO_MISSILE     = 70852,

    SPELL_CHOKING_GAS_BOMB          = 71255,
    SPELL_CHOKING_GAS_BOMB_AURA     = 71259,
    SPELL_CHOKING_GAS_BOMB_EXPL_AUR = 71280,
    SPELL_CHOKING_GAS_EXPLOSION     = 71279,

    // phase transitions
    SPELL_TEAR_GAS_1                = 71615,
    SPELL_TEAR_GAS                  = 71617,
    SPELL_TEAR_GAS_2                = 71618,
    SPELL_TEAR_GAS_PERIODIC_AURA    = 73170,
    SPELL_TEAR_GAS_CANCEL           = 71620,

    SPELL_CREATE_CONCOCTION         = 71621,
    SPELL_GUZZLE_POTIONS            = 71893,

    SPELL_MUTATED_PLAGUE            = 72451,

    // heroic
    SPELL_UNBOUND_PLAGUE            = 70911,
    SPELL_OOZE_VARIABLE             = 70352, // aura 303 - dont allow taking damage from attacker with linked aura303?
    SPELL_OOZE_VARIABLE_OOZE        = 74118, // anyway, implemented as hardcoded in script
    SPELL_GAS_VARIABLE              = 70353,
    SPELL_GAS_VARIABLE_GAS          = 74119,

    NPC_GREEN_ORANGE_OOZE_STALKER   = 37824, // casts orange and green visual spell and then summons add
    NPC_GROWING_OOZE_PUDDLE         = 37690,
    NPC_GROWING_OOZE_PUDDLE_TRIG    = 38234,
    NPC_CHOKING_GAS_BOMB            = 38159,
    NPC_VOLATILE_OOZE               = 37697,
    NPC_MUTATED_ABOMINATION         = 37672,
    NPC_MALLEABLE_GOO               = 38556,

/*
    SPELL_OOZE_GAS_PROTECTION     = 70812,
    SPELL_OOZE_BEAM_PROTECTION    = 71530,
    SPELL_OOZE_TANK_PROTECTION    = 71770,

    SPELL_ORANGE_RADIATION        = 45857, //Additional visual

    QUEST_24749                   = 71518,
    SHADOW_INFUSION_AURA          = 71516,

    VIEW_1                        = 30881,
    VIEW_2                        = 30881,
    VIEW_3                        = 30993,
*/
};

// talks
enum
{
    SAY_AGGRO                   = -1631092,
    SAY_AIRLOCK                 = -1631093,
    SAY_PHASE_CHANGE            = -1631094,
    SAY_TRANSFORM_1             = -1631095,
    SAY_TRANSFORM_2             = -1631096,
    SAY_SLAY_1                  = -1631097,
    SAY_SLAY_2                  = -1631098,
    SAY_BERSERK                 = -1631099,
    SAY_DEATH                   = -1631100,

    // during other fights
    SAY_SLIME_FLOW_1            = -1631074,
    SAY_SLIME_FLOW_2            = -1631075,
    SAY_BLIGHT                  = -1631083,
    SAY_FESTERGUT_DEATH         = -1631091,
    SAY_ROTFACE_DEATH           = -1631080,
};

static Locations SpawnLoc[]=
{
    {4356.779785f, 3263.510010f, 389.398010f, 1.586f},  // 0 Putricide start point o=1.586
    {4295.081055f, 3188.883545f, 389.330261f, 4.270f},  // 1 Puticide Festergut say, o=4.27
    {4417.302246f, 3188.219971f, 389.332520f, 5.102f},  // 2 Putricide Rotface say o=5.102
    {4388.1f, 3213.29f, 408.7399f, 3.8397f},            // 3 Ooze stalker - green
    {4324.7212f, 3214.6428f, 408.7399f, 5.5764f},       // 3 Ooze stalker - orange
};

#define POINT_PUTRICIDE_SPAWN 1

enum Phases
{
    PHASE_ONE = 1,
    PHASE_RUNNING_ONE = 2,
    PHASE_TRANSITION_ONE = 3,
    PHASE_TWO = 4,
    PHASE_RUNNING_TWO = 5,
    PHASE_TRANSITION_TWO = 6,
    PHASE_THREE = 7
};


/**
 * boss_professor_putricide
 */
struct MANGOS_DLL_DECL boss_professor_putricideAI : public base_icc_bossAI
{
    boss_professor_putricideAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiPhase;

    uint32 m_uiHealthCheckTimer;
    uint32 m_uiTransitionTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiPuddleTimer;
    uint32 m_uiUnstableExperimentTimer;
    uint32 m_uiMalleableGooTimer;
    uint32 m_uiChokingGasBombTimer;
    uint32 m_uiMutatedPlagueTimer;
    uint32 m_uiUnboundPlagueTimer;

    // used to determine whether he is assisting one of his pupils or having his own encounter
    bool m_bIsAssistingOnly;

    bool m_bIsGreenOoze; // green or orange ooze to summon

    void Reset()
    {
        m_uiPhase                   = PHASE_ONE;
        m_bIsAssistingOnly          = false;
        SetCombatMovement(true);

        m_bIsGreenOoze              = true; // first ooze summoned is always green

        m_uiHealthCheckTimer        = 1000;
        m_uiTransitionTimer         = 15000;
        m_uiEnrageTimer             = 10 * MINUTE * IN_MILLISECONDS;
        m_uiPuddleTimer             = 10000;
        m_uiUnstableExperimentTimer = 20000;
        m_uiMalleableGooTimer       = 5000;
        m_uiChokingGasBombTimer     = urand(10000, 15000);
        m_uiMutatedPlagueTimer      = 0;
        m_uiUnboundPlagueTimer      = 10000;
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        if (m_bIsAssistingOnly)
            uiDamage = 0;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY_1 - urand(0, 1), m_creature);
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_FESTERGUT) == IN_PROGRESS || m_pInstance->GetData(TYPE_ROTFACE) == IN_PROGRESS)
        {
            SetCombatMovement(false);
            m_bIsAssistingOnly = true;
            return;
        }

        m_pInstance->SetData(TYPE_PUTRICIDE, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PUTRICIDE, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PUTRICIDE, FAIL);

        if (VehicleKit *pKit = m_creature->GetVehicleKit())
        {
            pKit->RemoveAllPassengers();
            pKit->Reset();
        }

        // some weird bug with not regenerating health after wipe ;/
        m_creature->SetHealth(m_creature->GetMaxHealth());
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (uiData == POINT_PUTRICIDE_SPAWN)
        {
            if (m_uiPhase == PHASE_RUNNING_ONE)
            {
                if (m_bIsHeroic)
                {
                    DoScriptText(SAY_PHASE_CHANGE, m_creature);
                    m_uiTransitionTimer = 30000;
                }
                else
                {
                    DoCastSpellIfCan(m_creature, SPELL_CREATE_CONCOCTION);
                    DoScriptText(SAY_TRANSFORM_1, m_creature);
                }

                m_uiPhase = PHASE_TRANSITION_ONE; // counter for entering phase 2
            }
            else if (m_uiPhase == PHASE_RUNNING_TWO)
            {
                if (m_bIsHeroic)
                {
                    DoScriptText(SAY_PHASE_CHANGE, m_creature);
                    m_uiTransitionTimer = 30000;
                }
                else
                {
                    DoCastSpellIfCan(m_creature, SPELL_GUZZLE_POTIONS);
                    DoScriptText(SAY_TRANSFORM_2, m_creature);
                    m_uiPhase = PHASE_TRANSITION_TWO; // counter for entering phase 3
                }
            }
        }
    }

    void DoExperiment(bool green, bool both = false)
    {
        if (green || both)
        {
            if (Unit *pGreen = m_creature->SummonCreature(NPC_GREEN_ORANGE_OOZE_STALKER, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, SpawnLoc[3].o, TEMPSUMMON_TIMED_DESPAWN, 10000))
                DoCastSpellIfCan(m_creature, SPELL_GREEN_OOZE_SUMMON, CAST_TRIGGERED);
        }

        if (!green || both)
        {
            if (Unit *pOrange = m_creature->SummonCreature(NPC_GREEN_ORANGE_OOZE_STALKER, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, SpawnLoc[4].o, TEMPSUMMON_TIMED_DESPAWN, 10000))
                DoCastSpellIfCan(m_creature, SPELL_ORANGE_OOZE_SUMMON, CAST_TRIGGERED);
        }

        // variable
        if (both && m_bIs25Man)
            DoVariable();
    }

    void DoVariable()
    {
        std::list<Unit*> lGasVariables, lOozeVariables;
        bool bIsOoze = true;
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();

        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            if (!(*i)->getUnitGuid().IsPlayer())
                continue;

            if (Unit* pTmp = m_creature->GetMap()->GetUnit((*i)->getUnitGuid()))
            {
                if (pTmp->GetVehicle())
                    continue;
                else
                {
                    if (bIsOoze)
                        lGasVariables.push_back(pTmp);
                    else
                        lOozeVariables.push_back(pTmp);

                    bIsOoze = !bIsOoze;
                }
            }
        }

        // gas vars
        for(std::list<Unit*>::iterator i = lGasVariables.begin(); i != lGasVariables.end(); ++i)
            (*i)->CastSpell(*i, SPELL_GAS_VARIABLE, true);

        // ooze vars
        for(std::list<Unit*>::iterator i = lOozeVariables.begin(); i != lOozeVariables.end(); ++i)
            (*i)->CastSpell(*i, SPELL_OOZE_VARIABLE, true);
    }

    void JustSummoned(Creature *pSummoned)
    {
        if (pSummoned->GetEntry() != NPC_GREEN_ORANGE_OOZE_STALKER)
            pSummoned->SetInCombatWithZone();

        switch (pSummoned->GetEntry())
        {
            case NPC_GROWING_OOZE_PUDDLE:
            {
                pSummoned->CastSpell(pSummoned, SPELL_SLIME_PUDDLE_AURA, true);
                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                break;
            }
            case NPC_CHOKING_GAS_BOMB:
            {
                pSummoned->CastSpell(pSummoned, SPELL_CHOKING_GAS_BOMB_AURA, true);
                // creature is already despawned and spell doesn't hit anybody, this need proper implementation in core
                // pSummoned->CastSpell(pSummoned, SPELL_CHOKING_GAS_BOMB_EXPL_AUR, true);
                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsAssistingOnly)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Enrage
        if (m_uiEnrageTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiEnrageTimer = 30 * MINUTE * IN_MILLISECONDS;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        switch(m_uiPhase)
        {
            case PHASE_ONE:
            {
                // health check
                if (m_uiHealthCheckTimer <= uiDiff)
                {
                    if (m_creature->GetHealthPercent() <= 80.0f)
                    {
                        if (m_bIsHeroic)
                        {
                            DoCastSpellIfCan(m_creature, SPELL_VOLATILE_EXPERIMENT);
                            DoExperiment(true, true);
                            DoScriptText(SAY_PHASE_CHANGE, m_creature);
                        }
                        else
                        {
                            DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS, CAST_TRIGGERED);
                            DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS_1, CAST_TRIGGERED);
                        }

                        if (m_pInstance)
                            m_pInstance->SetData(TYPE_PUTRICIDE, SPECIAL);

                        m_creature->GetMotionMaster()->MovePoint(POINT_PUTRICIDE_SPAWN, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                        m_uiPhase = PHASE_RUNNING_ONE;
                        return;
                    }
                    m_uiHealthCheckTimer = 1000;
                }
                else
                    m_uiHealthCheckTimer -= uiDiff;

                // Unbound Plague
                if (m_bIsHeroic)
                {
                    if (m_uiUnboundPlagueTimer <= uiDiff)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_UNBOUND_PLAGUE, SELECT_FLAG_PLAYER))
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_UNBOUND_PLAGUE) == CAST_OK)
                                m_uiUnboundPlagueTimer = 70000;
                        }
                    }
                    else
                        m_uiUnboundPlagueTimer -= uiDiff;
                }

                // Slime Puddle
                if (m_uiPuddleTimer <= uiDiff)
                {
                    for (int i = 0; i < 2; ++i)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SLIME_PUDDLE_SUMMON, SELECT_FLAG_PLAYER))
                            DoCastSpellIfCan(pTarget, SPELL_SLIME_PUDDLE, CAST_TRIGGERED);
                    }

                    m_uiPuddleTimer = 30000;
                }
                else
                    m_uiPuddleTimer -= uiDiff;

                // Unstable Experiment
                if (m_uiUnstableExperimentTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_UNSTABLE_EXPERIMENT) == CAST_OK)
                    {
                        DoExperiment(m_bIsGreenOoze);
                        m_uiUnstableExperimentTimer = 30000;
                        m_bIsGreenOoze = !m_bIsGreenOoze;
                    }
                }
                else
                    m_uiUnstableExperimentTimer -= uiDiff;

                break;
            }
            case PHASE_TRANSITION_ONE:
            {
                if (m_uiTransitionTimer <= uiDiff)
                {
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiTransitionTimer = 15000;
                    m_uiPhase = PHASE_TWO;

                    if (m_pInstance)
                            m_pInstance->SetData(TYPE_PUTRICIDE, IN_PROGRESS);

                    if (m_bIsHeroic)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_CREATE_CONCOCTION);
                        DoScriptText(SAY_TRANSFORM_1, m_creature);
                    }
                    else
                        DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS_CANCEL, CAST_TRIGGERED);
                }
                else
                    m_uiTransitionTimer -= uiDiff;

                return;
            }
            case PHASE_TWO:
            {
                // health check
                if (m_uiHealthCheckTimer <= uiDiff)
                {
                    if (m_creature->GetHealthPercent() <= 35.0f)
                    {
                        if (m_bIsHeroic)
                        {
                            DoCastSpellIfCan(m_creature, SPELL_VOLATILE_EXPERIMENT);
                            DoExperiment(true, true);
                            DoScriptText(SAY_PHASE_CHANGE, m_creature);
                        }
                        else
                        {
                            DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS, CAST_TRIGGERED);
                            DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS_1, CAST_TRIGGERED);
                        }

                        if (m_pInstance)
                            m_pInstance->SetData(TYPE_PUTRICIDE, SPECIAL);

                        m_creature->GetMotionMaster()->MovePoint(POINT_PUTRICIDE_SPAWN, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                        m_uiPhase = PHASE_RUNNING_TWO;
                        return;
                    }
                    m_uiHealthCheckTimer = 1000;
                }
                else
                    m_uiHealthCheckTimer -= uiDiff;

                // Unbound Plague
                if (m_bIsHeroic)
                {
                    if (m_uiUnboundPlagueTimer <= uiDiff)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_UNBOUND_PLAGUE, SELECT_FLAG_PLAYER))
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_UNBOUND_PLAGUE) == CAST_OK)
                                m_uiUnboundPlagueTimer = 70000;
                        }
                    }
                    else
                        m_uiUnboundPlagueTimer -= uiDiff;
                }

                // Slime Puddle
                if (m_uiPuddleTimer <= uiDiff)
                {
                    for (int i = 0; i < 2; ++i)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SLIME_PUDDLE_SUMMON, SELECT_FLAG_PLAYER))
                            DoCastSpellIfCan(pTarget, SPELL_SLIME_PUDDLE, CAST_TRIGGERED);
                    }

                    m_uiPuddleTimer = 30000;
                }
                else
                    m_uiPuddleTimer -= uiDiff;

                // Unstable Experiment
                if (m_uiUnstableExperimentTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_UNSTABLE_EXPERIMENT) == CAST_OK)
                    {
                        DoExperiment(m_bIsGreenOoze);
                        m_uiUnstableExperimentTimer = 30000;
                        m_bIsGreenOoze = !m_bIsGreenOoze;
                    }
                }
                else
                    m_uiUnstableExperimentTimer -= uiDiff;

                // Malleable Goo
                if (m_uiMalleableGooTimer <= uiDiff)
                {
                    for (int i = 0; i < (m_bIs25Man ? 2 : 1); ++i)
                    {
                        if (Unit *pTarget = SelectRandomRangedTarget(m_creature))
                        {
                            float x, y, z;
                            pTarget->GetPosition(x, y, z);
                            if (Creature *pTmp = m_creature->SummonCreature(NPC_MALLEABLE_GOO, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000))
                            {
                                DoCastSpellIfCan(pTmp, SPELL_MALLEABLE_GOO_MISSILE);
                                m_uiMalleableGooTimer = 20000;
                            }
                        }
                    }
                }
                else
                    m_uiMalleableGooTimer -= uiDiff;

                // Choking Gas
                if (m_uiChokingGasBombTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_CHOKING_GAS_BOMB) == CAST_OK)
                        m_uiChokingGasBombTimer = urand(25000, 30000);
                }
                else
                    m_uiChokingGasBombTimer -= uiDiff;

                break;
            }
            case PHASE_TRANSITION_TWO:
            {
                if (m_uiTransitionTimer <= uiDiff)
                {
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiTransitionTimer = 15000;
                    m_uiPhase = PHASE_THREE;

                    if (m_pInstance)
                            m_pInstance->SetData(TYPE_PUTRICIDE, IN_PROGRESS);

                    if (m_bIsHeroic)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_GUZZLE_POTIONS);
                        DoScriptText(SAY_TRANSFORM_2, m_creature);
                    }
                    else
                        DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS_CANCEL, CAST_TRIGGERED);
                }
                else
                    m_uiTransitionTimer -= uiDiff;

                return;
            }
            case PHASE_THREE:
            {
                // Unbound Plague
                if (m_bIsHeroic)
                {
                    if (m_uiUnboundPlagueTimer <= uiDiff)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_UNBOUND_PLAGUE, SELECT_FLAG_PLAYER))
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_UNBOUND_PLAGUE) == CAST_OK)
                                m_uiUnboundPlagueTimer = 70000;
                        }
                    }
                    else
                        m_uiUnboundPlagueTimer -= uiDiff;
                }

                // Mutated Plague (proc cooldown for creatures doesn't work)
                if (m_uiMutatedPlagueTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MUTATED_PLAGUE) == CAST_OK)
                        m_uiMutatedPlagueTimer = 10000;
                }
                else
                    m_uiMutatedPlagueTimer -= uiDiff;

                // Slime Puddle
                if (m_uiPuddleTimer <= uiDiff)
                {
                    for (int i = 0; i < 2; ++i)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SLIME_PUDDLE_SUMMON, SELECT_FLAG_PLAYER))
                            DoCastSpellIfCan(pTarget, SPELL_SLIME_PUDDLE, CAST_TRIGGERED);
                    }

                    m_uiPuddleTimer = 30000;
                }
                else
                    m_uiPuddleTimer -= uiDiff;

                // Malleable Goo
                if (m_uiMalleableGooTimer <= uiDiff)
                {
                    for (int i = 0; i < (m_bIs25Man ? 2 : 1); ++i)
                    {
                        if (Unit *pTarget = SelectRandomRangedTarget(m_creature))
                        {
                            float x, y, z;
                            pTarget->GetPosition(x, y, z);
                            if (Creature *pTmp = m_creature->SummonCreature(NPC_MALLEABLE_GOO, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000))
                            {
                                DoCastSpellIfCan(pTmp, SPELL_MALLEABLE_GOO_MISSILE);
                                m_uiMalleableGooTimer = 20000;
                            }
                        }
                    }
                }
                else
                    m_uiMalleableGooTimer -= uiDiff;

                // Choking Gas
                if (m_uiChokingGasBombTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_CHOKING_GAS_BOMB) == CAST_OK)
                        m_uiChokingGasBombTimer = urand(25000, 30000);
                }
                else
                    m_uiChokingGasBombTimer -= uiDiff;

                break;
            }
            case PHASE_RUNNING_ONE:
            case PHASE_RUNNING_TWO:
                // wait for arriving at the table (during phase transition)
                break;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_professor_putricide(Creature* pCreature)
{
    return new boss_professor_putricideAI(pCreature);
}

/**
 * mob_icc_gas_cloud
 */
struct MANGOS_DLL_DECL mob_icc_gas_cloudAI : public ScriptedAI
{
    mob_icc_gas_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_creature->SetInCombatWithZone();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsVariable = false;
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_PUTRICIDE) == SPECIAL)
            {
                DoCastSpellIfCan(m_creature, SPELL_GAS_VARIABLE_GAS, CAST_TRIGGERED);
                m_bIsVariable = true;
            }
        }
        m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 m_uiWaitTimer;
    uint32 m_uiMoveTimer;
    bool m_bIsWaiting;
    bool m_bIsVariable;

    void Reset()
    {
        m_bIsWaiting    = true;
        m_uiWaitTimer   = 1000;
        m_uiMoveTimer   = 4000;
        SetCombatMovement(false);
        DoCastSpellIfCan(m_creature, SPELL_GASEOUS_BLOAT_VISUAL, CAST_TRIGGERED);
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        if (m_bIsVariable)
        {
            if (pDealer->HasAura(SPELL_GAS_VARIABLE, EFFECT_INDEX_0))
            {
                uiDamage = 0;
                return;
            }
        }

        if (uiDamage > m_creature->GetHealth())
        {
            // remove variable debuffs
            if (m_bIsVariable)
            {
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();

                for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
                {
                    if (!(*i)->getUnitGuid().IsPlayer())
                        continue;

                    if (Unit* pTmp = m_creature->GetMap()->GetUnit((*i)->getUnitGuid()))
                        pTmp->RemoveAurasDueToSpell(SPELL_GAS_VARIABLE);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            uint32 data = m_pInstance->GetData(TYPE_PUTRICIDE);
            if (data != IN_PROGRESS && data != SPECIAL)
                m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsWaiting)
        {
            if (m_uiWaitTimer <= uiDiff)
            {
                m_uiWaitTimer = 5000;

                // pick target
                if (m_pInstance)
                {
                    if (Creature *pProf = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                    {
                        Unit *pTarget = NULL;

                        pTarget = pProf->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_GASEOUS_BLOAT, SELECT_FLAG_PLAYER);
                        if (!pTarget)
                            pTarget = pProf->getVictim();

                        if (pTarget)
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_GASEOUS_BLOAT) == CAST_OK)
                            {
                                DoResetThreat();
                                m_creature->AddThreat(pTarget, 1000000.0f);
                            }
                        }
                    }
                }
            }
            else
                m_uiWaitTimer -= uiDiff;

            if (m_uiMoveTimer <= uiDiff)
            {
                SetCombatMovement(true);
                m_bIsWaiting = false;
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            else
                m_uiMoveTimer -= uiDiff;

            return;
        }
        else
        {
            // follow the victim - problems with updating the moving while channeling the spell
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }

        if (m_creature->GetDistance(m_creature->getVictim()) <= 4.0f)
        {
            m_creature->getVictim()->CastSpell(m_creature->getVictim(), SPELL_EXPUNGED_GAS, true);
            m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->Clear();
            m_bIsWaiting = true;
            m_uiWaitTimer = 2000;
            m_uiMoveTimer = 5000;
        }
    }
};
CreatureAI* GetAI_mob_icc_gas_cloud(Creature* pCreature)
{
    return new mob_icc_gas_cloudAI(pCreature);
}

/**
 * mob_icc_volatile_ooze
 */
struct MANGOS_DLL_DECL mob_icc_volatile_oozeAI : public base_icc_bossAI
{
    mob_icc_volatile_oozeAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        m_creature->SetInCombatWithZone();
        m_bIsVariable = false;
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_PUTRICIDE) == SPECIAL)
            {
                DoCastSpellIfCan(m_creature, SPELL_OOZE_VARIABLE_OOZE, CAST_TRIGGERED);
                m_bIsVariable = true;
            }
        }
        m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        Reset();
    }

    uint32 m_uiWaitTimer;
    uint32 m_uiMoveTimer;
    bool m_bIsWaiting;
    bool m_bIsVariable;

    void Reset()
    {
        m_bIsWaiting    = true;
        m_uiWaitTimer   = 1000;
        m_uiMoveTimer   = 4000;
        SetCombatMovement(false);
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        if (m_bIsVariable)
        {
            if (pDealer->HasAura(SPELL_OOZE_VARIABLE, EFFECT_INDEX_0))
            {
                uiDamage = 0;
                return;
            }
        }

        if (uiDamage >= m_creature->GetHealth())
        {
            // remove variable debuffs
            if (m_bIsVariable)
            {
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();

                for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
                {
                    if (!(*i)->getUnitGuid().IsPlayer())
                        continue;

                    if (Unit* pTmp = m_creature->GetMap()->GetUnit((*i)->getUnitGuid()))
                        pTmp->RemoveAurasDueToSpell(SPELL_OOZE_VARIABLE);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            uint32 data = m_pInstance->GetData(TYPE_PUTRICIDE);
            if (data != IN_PROGRESS && data != SPECIAL)
                m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsWaiting)
        {
            if (m_uiWaitTimer <= uiDiff)
            {
                m_uiWaitTimer = 5000;

                // pick target
                if (m_pInstance)
                {
                    if (Creature *pProf = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                    {
                        Unit *pTarget = NULL;

                        pTarget = pProf->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_OOZE_ADHESIVE, SELECT_FLAG_PLAYER);
                        if (!pTarget)
                            pTarget = pProf->getVictim();

                        if (pTarget)
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_OOZE_ADHESIVE) == CAST_OK)
                            {
                                DoResetThreat();
                                m_creature->AddThreat(pTarget, 1000000.0f);
                            }
                        }
                    }
                }
            }
            else
                m_uiWaitTimer -= uiDiff;

            if (m_uiMoveTimer <= uiDiff)
            {
                SetCombatMovement(true);
                m_bIsWaiting = false;
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            else
                m_uiMoveTimer -= uiDiff;

            return;
        }
        else
        {
            // follow the victim - problems with updating the moving while channeling the spell
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }

        if (m_creature->GetDistance(m_creature->getVictim()) <= 4.0f)
        {
            m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_OOZE_ERUPTION);
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->Clear();
            m_bIsWaiting = true;
            m_uiWaitTimer = 2000;
            m_uiMoveTimer = 5000;
        }
    }
};
CreatureAI* GetAI_mob_icc_volatile_ooze(Creature* pCreature)
{
    return new mob_icc_volatile_oozeAI(pCreature);
}

// passive mob AI struct
struct MANGOS_DLL_DECL mob_choking_gas_bombAI : public ScriptedAI
{
    mob_choking_gas_bombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_uiExplosionTimer = 9500;
    }

    uint32 m_uiExplosionTimer;

    void Reset(){}
    void AttackStart(Unit *pWho){}
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiExplosionTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_CHOKING_GAS_EXPLOSION, CAST_TRIGGERED);
            m_uiExplosionTimer = 9500;
        }
        else
            m_uiExplosionTimer -= uiDiff;
    }
};
CreatureAI* GetAI_mob_choking_gas_bomb(Creature* pCreature)
{
    return new mob_choking_gas_bombAI(pCreature);
}

// passive mob AI struct
struct MANGOS_DLL_DECL mob_ooze_puddleAI : public ScriptedAI
{
    mob_ooze_puddleAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetObjectScale(0.2f);
        DoCastSpellIfCan(m_creature, SPELL_GROW_STACKER, CAST_TRIGGERED);
    }

    ScriptedInstance *m_pInstance;

    void Reset(){}
    void AttackStart(Unit *pWho){}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            uint32 data = m_pInstance->GetData(TYPE_PUTRICIDE);
            if (data == SPECIAL)
            {
                // don't grow while Putricide is mutating between phases
                return;
            }
            else if (data != IN_PROGRESS)
                m_creature->ForcedDespawn();
        }
    }
};
CreatureAI* GetAI_mob_ooze_puddle(Creature* pCreature)
{
    return new mob_ooze_puddleAI(pCreature);
}

void AddSC_boss_professor_putricide()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_professor_putricide";
    newscript->GetAI = &GetAI_boss_professor_putricide;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_volatile_ooze";
    newscript->GetAI = &GetAI_mob_icc_volatile_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_gas_cloud";
    newscript->GetAI = &GetAI_mob_icc_gas_cloud;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_choking_gas_bomb";
    newscript->GetAI = &GetAI_mob_choking_gas_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ooze_puddle";
    newscript->GetAI = &GetAI_mob_ooze_puddle;
    newscript->RegisterSelf();
}
