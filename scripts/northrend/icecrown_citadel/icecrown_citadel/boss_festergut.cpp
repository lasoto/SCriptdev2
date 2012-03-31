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
SDName: boss_festergut
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SPELL_BERSERK               = 47008,

    // Gastric Bloat
    SPELL_GASTRIC_BLOAT         = 72214, // proc aura, ~8 sec cooldown, currently not used
    SPELL_GASTRIC_BLOAT_TRIG    = 72219,

    // Inhale Blight
    SPELL_INHALE_BLIGHT         = 69165,
    SPELL_INHALED_BLIGHT_10     = 69166,
    SPELL_INHALED_BLIGHT_25     = 71912,

    // Pungent Blight
    SPELL_PUNGENT_BLIGHT        = 69195,

    // Gaseous Blight
    // periodic auras spells
    SPELL_GASEUS_BLIGHT_DUMMY   = 69125, // gas is spread into the room on aggro
    SPELL_GASEOUS_BLIGHT_1      = 69157,
    SPELL_GASEOUS_BLIGHT_2      = 69162,
    SPELL_GASEOUS_BLIGHT_3      = 69164,
    // visual gas dummy auras
    SPELL_GASEOUS_BLIGHT_DUMMY1 = 69126,
    SPELL_GASEOUS_BLIGHT_DUMMY2 = 69152,
    SPELL_GASEOUS_BLIGHT_DUMMY3 = 69154,

    // Inoculent
    SPELL_REMOVE_INOCULENT      = 69298,

    // Gas Spore
    SPELL_GAS_SPORE             = 69278,

    // Vile Gas
    SPELL_VILE_GAS_SUMMON       = 72288,
    SPELL_VILE_GAS_SUMMON_TRIG  = 72287,
    SPELL_VILE_GAS              = 71307,
    SPELL_VILE_GAS_TRIGGERED    = 69240,

    // Malleable Goo
    SPELL_MALLEABLE_GOO_SUMMON  = 72299,
    SPELL_MALLEABLE_GOO         = 72295,
    SPELL_MALLEABLE_GOO_VISUAL  = 75845,
    SPELL_MALLEABLE_GOO_MISSILE = 70852,

    // other
    NPC_ORANGE_GAS_STALKER      = 36659, // has dummy auras of the orange gas
    NPC_PUDDLE_STALKER          = 37013, // dummy npc for initial gas flowing from pipes animation
    NPC_MALLEABLE_GOO           = 38556,
};

// talks
enum
{
    SAY_STINKY_DIES             = -1631081,
    SAY_AGGRO                   = -1631082,
    SAY_BLIGHT                  = -1631083,
    SAY_SPORE                   = -1631084,
    SAY_PUNGUENT_BLIGHT         = -1631085,
    SAY_PUNGUENT_BLIGHT_EMOTE   = -1631086,
    SAY_SLAY_1                  = -1631087,
    SAY_SLAY_2                  = -1631088,
    SAY_BERSERK                 = -1631089,
    SAY_DEATH                   = -1631090,
};

static Locations SpawnLoc[]=
{
    {4322.85f, 3164.17f, 389.40f, 3.76f},               // festergut side
    {4311.91f, 3157.42f, 389.00f, 3.62f},               // hacky (LoS problems?) festergut side
    {4391.38f, 3163.71f, 389.40f, 5.8f}                 // rotface side
};

struct MANGOS_DLL_DECL boss_festergutAI : public base_icc_bossAI
{
    boss_festergutAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiBerserkTimer;
    uint32 m_uiGastricBloatTimer;
    uint32 m_uiInhaleBlightTimer;
    uint32 m_uiGasSporeTimer;
    uint32 m_uiVileGasTimer;
    uint32 m_uiMalleableGooTimer;

    void Reset()
    {
        m_uiBerserkTimer = 5 * MINUTE * IN_MILLISECONDS;
        m_uiGastricBloatTimer = 10000;
        m_uiInhaleBlightTimer = 30000;
        m_uiGasSporeTimer = 20000;
        m_uiVileGasTimer = 10000;
        m_uiMalleableGooTimer = urand(15000, 20000);
    }

    void Aggro(Unit *pWho)
    {
        // not working as intended currently
        // DoCastSpellIfCan(m_creature, SPELL_GASTRIC_BLOAT, CAST_TRIGGERED);

        DoCastSpellIfCan(m_creature, SPELL_GASEOUS_BLIGHT_1, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_GASEUS_BLIGHT_DUMMY, CAST_TRIGGERED);

        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_FESTERGUT, IN_PROGRESS);

            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
            {
                pProfessor->NearTeleportTo(SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, SpawnLoc[1].o);
                pProfessor->SetInCombatWithZone();
            }
        }
    }

    void KilledUnit(Unit *pVictim)
    {
        DoScriptText(SAY_SLAY_1 - urand(0, 1), m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_FESTERGUT, FAIL);

            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                pProfessor->AI()->EnterEvadeMode();
        }

        DoCastSpellIfCan(m_creature, SPELL_REMOVE_INOCULENT, CAST_TRIGGERED);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_FESTERGUT, DONE);

            if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                pProfessor->AI()->EnterEvadeMode();
        }

        DoScriptText(SAY_DEATH, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_REMOVE_INOCULENT, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Berserk
        if (m_uiBerserkTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiBerserkTimer = 5 * MINUTE * IN_MILLISECONDS;
            }
        }
        else
            m_uiBerserkTimer -= uiDiff;

        // Gastric Bloat
        if (m_uiGastricBloatTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GASTRIC_BLOAT_TRIG) == CAST_OK)
                m_uiGastricBloatTimer = 10000;
        }
        else
            m_uiGastricBloatTimer -= uiDiff;

        // Inhale Blight, Pungent Blight
        if (m_uiInhaleBlightTimer <= uiDiff)
        {
            // check for Pungent Blight
            SpellAuraHolderPtr holder = m_creature->GetSpellAuraHolder(SPELL_INHALED_BLIGHT_10);
            if (!holder)
                holder = m_creature->GetSpellAuraHolder(SPELL_INHALED_BLIGHT_25);
            if (holder)
            {
                if (holder->GetStackAmount() >= 3)
                {
                    // can't inhale anymore...
                    if (DoCastSpellIfCan(m_creature, SPELL_PUNGENT_BLIGHT) == CAST_OK)
                    {
                        DoScriptText(SAY_PUNGUENT_BLIGHT_EMOTE, m_creature);
                        DoScriptText(SAY_PUNGUENT_BLIGHT, m_creature);
                        m_uiInhaleBlightTimer = 35000;
                    }

                    return;
                }
            }

            if (DoCastSpellIfCan(m_creature, SPELL_INHALE_BLIGHT) == CAST_OK)
            {
                if (m_pInstance)
                {
                    if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                        DoScriptText(SAY_BLIGHT, pProfessor);
                }
                m_uiInhaleBlightTimer = 30000;
            }
        }
        else
            m_uiInhaleBlightTimer -= uiDiff;

        // Gas Spore
        if (m_uiGasSporeTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_GAS_SPORE) == CAST_OK)
            {
                DoScriptText(SAY_SPORE, m_creature);
                m_uiGasSporeTimer = 40000;
                
                // make sure Vile Gas is not cast when players are gathered near Gas Spore
                if (m_uiVileGasTimer < 12000)
                    m_uiVileGasTimer = 12000;
            }
        }
        else
            m_uiGasSporeTimer -= uiDiff;

        // Vile Gas
        if (m_uiVileGasTimer <= uiDiff)
        {
            // DoCastSpellIfCan(m_creature, SPELL_VILE_GAS_SUMMON, CAST_TRIGGERED);
            // DoCastSpellIfCan(m_creature, SPELL_VILE_GAS, CAST_TRIGGERED);

            if (Unit *pTarget = SelectRandomRangedTarget(m_creature))
            {
                pTarget->CastSpell(pTarget, SPELL_VILE_GAS_SUMMON_TRIG, true);
                DoCastSpellIfCan(m_creature, SPELL_VILE_GAS, CAST_TRIGGERED);
                m_uiVileGasTimer = 30000;
            }
        }
        else
            m_uiVileGasTimer -= uiDiff;

        // Malleable Goo
        if (m_bIsHeroic)
        {
            if (m_uiMalleableGooTimer <= uiDiff)
            {
                if (Creature *pProfessor = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                {
                    if (Unit *pTarget = SelectRandomRangedTarget(m_creature))
                    {
                        // pProfessor->CastSpell(m_creature, SPELL_MALLEABLE_GOO_SUMMON, true);
                        // pProfessor->CastSpell(m_creature, SPELL_MALLEABLE_GOO, true);

                        float x, y, z;
                        pTarget->GetPosition(x, y, z);
                        if (Creature *pTmp = m_creature->SummonCreature(NPC_MALLEABLE_GOO, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000))
                        {
                            pProfessor->CastSpell(pTmp, SPELL_MALLEABLE_GOO_MISSILE, false);
                            m_uiMalleableGooTimer = urand(15000, 20000);
                        }
                    }
                }
            }
            else
                m_uiMalleableGooTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_festergut(Creature* pCreature)
{
    return new boss_festergutAI(pCreature);
}

struct MANGOS_DLL_DECL mob_vile_gas_malleable_gooAI : public ScriptedAI
{
    mob_vile_gas_malleable_gooAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
    }
    void DamageTaken(Unit *pDealer, uint32 &uiDamage){ uiDamage = 0; }
    void Reset(){}
    void AttackStart(Unit *pWho){}
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_vile_gas_malleable_goo(Creature* pCreature)
{
    return new mob_vile_gas_malleable_gooAI(pCreature);
}

void AddSC_boss_festergut()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_festergut";
    newscript->GetAI = &GetAI_boss_festergut;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vile_gas_malleable_goo";
    newscript->GetAI = &GetAI_mob_vile_gas_malleable_goo;
    newscript->RegisterSelf();
}
