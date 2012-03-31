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
SDName: boss_rotface
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
    SPELL_BERSERK               = 47008,

    // Mutated Infection
    SPELL_MUTATED_INFECTION_1   = 70090, // periodic trigger auras
    SPELL_MUTATED_INFECTION_2   = 70003,
    SPELL_MUTATED_INFECTION_3   = 70004,
    SPELL_MUTATED_INFECTION_4   = 70005,
    SPELL_MUTATED_INFECTION_5   = 70006,

    // Slime Spray
    SPELL_SLIME_SPRAY_SUMMON    = 70882,
    SPELL_SLIME_SPRAY           = 69508,

    // Ooze Flood
    SPELL_OOZE_FLOOD_PERIODIC   = 70069, // periodically trigger ooze flood
    SPELL_OOZE_FLOOD_REMOVE     = 70079,

    // Little Ooze
    SPELL_STICKY_OOZE           = 69774,
    SPELL_STICKY_AURA           = 69776,
    SPELL_WEAK_RADIATING_OOZE   = 69750,
    SPELL_LITTLE_OOZE_COMBINE   = 69537, // periodic check
    SPELL_MERGE                 = 69889,

    // Big Ooze
    SPELL_UNSTABLE_OOZE         = 69558, // stacking buff
    SPELL_RADIATING_OOZE        = 69760,
    SPELL_BIG_OOZE_COMBINE      = 69552, // periodic check
    SPELL_BIG_OOZE_BUFF_COMB    = 69611, // periodic check

    // Vile Gas (heroic)
    SPELL_VILE_GAS_SUMMON       = 72288,
    SPELL_VILE_GAS_SUMMON_TRIG  = 72287,
    SPELL_VILE_GAS              = 71307,
    SPELL_VILE_GAS_TRIGGERED    = 72272,

    // others
    NPC_PUDDLE_STALKER          = 37013,
    NPC_LITTLE_OOZE             = 36897,
    NPC_BIG_OOZE                = 36899,
};

static uint32 uiMutatedInfections[5] =
{
    SPELL_MUTATED_INFECTION_1,
    SPELL_MUTATED_INFECTION_2,
    SPELL_MUTATED_INFECTION_3,
    SPELL_MUTATED_INFECTION_4,
    SPELL_MUTATED_INFECTION_5
};

//talks
enum
{
    SAY_PRECIOUS_DIES           = -1631070,
    SAY_AGGRO                   = -1631071,
    SAY_SLIME_SPRAY             = -1631072,
    SAY_OOZE_EXPLODE            = -1631073,
    SAY_SLIME_FLOW_1            = -1631074,
    SAY_SLIME_FLOW_2            = -1631075,
    SAY_SLAY_1                  = -1631076,
    SAY_SLAY_2                  = -1631077,
    SAY_BERSERK                 = -1631078,
    SAY_DEATH                   = -1631079,
};

static Locations SpawnLoc[]=
{
    {4322.85f, 3164.17f, 389.40f, 3.76f},               // festergut side
    {4311.91f, 3157.42f, 389.00f, 3.62f},               // hacky (LoS problems?) festergut side
    {4391.38f, 3163.71f, 389.40f, 5.8f}                 // rotface side
};

// Rotface
struct MANGOS_DLL_DECL boss_rotfaceAI : public base_icc_bossAI
{
    boss_rotfaceAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiSlimeSprayTimer;
    uint32 m_uiMutatedInfectionTimer;
    uint32 m_uiMutatedInfectionBeforeTimer;
    uint32 m_uiInfectionsRate;
    uint32 m_uiVileGasTimer;
    uint32 m_uiSlimeFlowTimer;

    void Reset()
    {
        m_uiSlimeSprayTimer = urand(17000, 23000);
        m_uiVileGasTimer = 20000;
        m_uiMutatedInfectionTimer = m_uiMutatedInfectionBeforeTimer = 60000;
        m_uiInfectionsRate = 1;
        m_uiSlimeFlowTimer = 20000;
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ROTFACE, IN_PROGRESS);

            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
            {
                pProfessor->NearTeleportTo(SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, SpawnLoc[2].o);
                pProfessor->SetInCombatWithZone();
            }
        }

        DoScriptText(SAY_AGGRO, m_creature);

        DoCastSpellIfCan(m_creature, SPELL_MUTATED_INFECTION_1, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_OOZE_FLOOD_PERIODIC, CAST_TRIGGERED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ROTFACE, FAIL);

            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                pProfessor->AI()->EnterEvadeMode();
        }

        DoCastSpellIfCan(m_creature, SPELL_OOZE_FLOOD_REMOVE, CAST_TRIGGERED);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_SLAY_1 - urand(0,1), m_creature, pVictim);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ROTFACE, DONE);

            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                pProfessor->AI()->EnterEvadeMode();
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Slime Spray
        if (m_uiSlimeSprayTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SLIME_SPRAY_SUMMON, CAST_TRIGGERED) == CAST_OK)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SLIME_SPRAY) == CAST_OK)
                {
                    DoScriptText(SAY_SLIME_SPRAY, m_creature);
                    m_uiSlimeSprayTimer = urand(17000, 23000);
                }
            }
        }
        else
            m_uiSlimeSprayTimer -= uiDiff;

        // Mutated Infection - faster with time
        // implemented this instead of phases
        if (m_uiInfectionsRate < 5)
        {
            if (m_uiMutatedInfectionTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, uiMutatedInfections[m_uiInfectionsRate], CAST_TRIGGERED) == CAST_OK)
                {
                    m_creature->RemoveAurasDueToSpell(uiMutatedInfections[m_uiInfectionsRate - 1]);
                    m_uiMutatedInfectionBeforeTimer = m_uiMutatedInfectionBeforeTimer - 10000; // every next 15 seconds faster
                    m_uiMutatedInfectionTimer = m_uiMutatedInfectionBeforeTimer;
                    ++m_uiInfectionsRate;
                }
            }
            else
                m_uiMutatedInfectionTimer -= uiDiff;
        }

        // Vile Gas
        if (m_bIsHeroic)
        {
            if (m_uiVileGasTimer <= uiDiff)
            {
                if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                {
                    if (Unit *pTarget = SelectRandomRangedTarget(m_creature))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_VILE_GAS_SUMMON_TRIG, CAST_TRIGGERED) == CAST_OK)
                        {
                            pProfessor->CastSpell(pProfessor, SPELL_VILE_GAS, true);
                            m_uiVileGasTimer = 20000;
                        }
                    }
                }
            }
            else
                m_uiVileGasTimer -= uiDiff;
        }

        // Slime Flow
        if (m_uiSlimeFlowTimer <= uiDiff)
        {
            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                DoScriptText(SAY_SLIME_FLOW_1 - urand(0, 1), pProfessor);

            m_uiSlimeFlowTimer = 20000;
        }
        else
            m_uiSlimeFlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rotface(Creature* pCreature)
{
    return new boss_rotfaceAI(pCreature);
}

// all passive dummy NPCs
struct MANGOS_DLL_DECL  mob_rotface_ooze_dummyAI : public ScriptedAI
{
    mob_rotface_ooze_dummyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
    }
    void Reset(){}
    void AttackStart(Unit *pWho){}
    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        uiDamage = 0;
    }
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_rotface_ooze_dummy(Creature* pCreature)
{
    return new mob_rotface_ooze_dummyAI(pCreature);
}

struct MANGOS_DLL_DECL mob_little_oozeAI : public ScriptedAI
{
    mob_little_oozeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiStickyOozeTimer;

    void Reset()
    {
        m_uiStickyOozeTimer = 5000;
    }

    void EnterEvadeMode()
    {
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit *pWho)
    {
        m_creature->AddThreat(pWho, 500000.0f);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
        DoCastSpellIfCan(m_creature, SPELL_WEAK_RADIATING_OOZE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_LITTLE_OOZE_COMBINE, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStickyOozeTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_STICKY_OOZE) == CAST_OK)
                m_uiStickyOozeTimer = urand(10000, 15000);
        }
        else
            m_uiStickyOozeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_little_ooze(Creature* pCreature)
{
    return new mob_little_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL mob_big_oozeAI : public ScriptedAI
{
    mob_big_oozeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiStickyOozeTimer;
    uint32 m_uiCheckTimer;
    bool m_bHasSaid;

    void Reset()
    {
        m_uiStickyOozeTimer = 5000;
        m_uiCheckTimer = 1000;
        m_bHasSaid = false;
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit *pWho)
    {
        m_creature->AddThreat(pWho, 500000.0f);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
        DoCastSpellIfCan(m_creature, SPELL_RADIATING_OOZE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_BIG_OOZE_COMBINE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_BIG_OOZE_BUFF_COMB, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Unstable Ooze
        if (!m_bHasSaid)
        {
            if (m_uiCheckTimer <= uiDiff)
            {
                if (SpellAuraHolderPtr holder= m_creature->GetSpellAuraHolder(SPELL_UNSTABLE_OOZE))
                {
                    if (holder->GetStackAmount() >= 5)
                    {
                        if (m_pInstance)
                        {
                            if (Creature *pRotface = m_pInstance->GetSingleCreatureFromStorage(NPC_ROTFACE))
                            {
                                DoScriptText(SAY_OOZE_EXPLODE, pRotface);
                                m_bHasSaid = true;
                            }
                        }
                    }
                }
                m_uiCheckTimer = 1000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }

        // Sticky Ooze
        if (m_uiStickyOozeTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_STICKY_OOZE) == CAST_OK)
                m_uiStickyOozeTimer = urand(10000, 15000);
        }
        else
            m_uiStickyOozeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_big_ooze(Creature* pCreature)
{
    return new mob_big_oozeAI(pCreature);
}

// Ooze explosion stalker
struct MANGOS_DLL_DECL mob_ooze_explosion_stalkerAI : public ScriptedAI
{
    mob_ooze_explosion_stalkerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pCreature->ForcedDespawn(10000);
    }
    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_ooze_explosion_stalker(Creature* pCreature)
{
    return new mob_ooze_explosion_stalkerAI(pCreature);
}

struct MANGOS_DLL_DECL mob_sticky_oozeAI : public ScriptedAI
{
    mob_sticky_oozeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        pCreature->CastSpell(pCreature, SPELL_STICKY_AURA, true);
    }

    ScriptedInstance *m_pInstance;

    void Reset(){}
    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        uiDamage = 0;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }
    }
};

CreatureAI* GetAI_mob_sticky_ooze(Creature* pCreature)
{
    return new mob_sticky_oozeAI(pCreature);
}

void AddSC_boss_rotface()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_rotface";
    newscript->GetAI = &GetAI_boss_rotface;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_rotface_ooze_dummy";
    newscript->GetAI = &GetAI_mob_rotface_ooze_dummy;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_little_ooze";
    newscript->GetAI = &GetAI_mob_little_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_big_ooze";
    newscript->GetAI = &GetAI_mob_big_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sticky_ooze";
    newscript->GetAI = &GetAI_mob_sticky_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ooze_explosion_stalker";
    newscript->GetAI = &GetAI_mob_ooze_explosion_stalker;
    newscript->RegisterSelf();
}
