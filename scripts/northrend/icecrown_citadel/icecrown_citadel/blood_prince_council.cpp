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
SDName: blood_prince_council
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
    // spells
    SPELL_BERSERK               = 26662,
    SPELL_FEIGN_DEATH           = 71598,

    SPELL_INVOCATION_V_MOVE     = 71075,
    SPELL_INVOCATION_K_MOVE     = 71079,
    SPELL_INVOCATION_T_MOVE     = 71082,

    // Valanar
    SPELL_INVOCATION_VALANAR    = 70952,

    SPELL_SHOCK_VORTEX          = 72037,
    SPELL_SHOCK_VORTEX_AURA     = 71945,
    SPELL_SHOCK_VORTEX_VISUAL   = 72633,

    SPELL_EMP_SHOCK_VORTEX      = 72039,

    SPELL_KINETIC_BOMB          = 72053, // summons the target, which must summon the bomb above its head and make it move to the target
    SPELL_KINETIC_BOMB_TARGET   = 72080,
    SPELL_KINETIC_BOMB_DMG      = 72052,
    SPELL_KINETIC_BOMB_VISUAL   = 72054,
    SPELL_UNSTABLE              = 72059, // this proc makes the bomb teleport by 3-5? yards up and applies knockback aura
    SPELL_KNOCKBACK_AURA        = 72087, // this stacks to 2. duration = 3 secs, maybe this makes bomb stand for 3secs per stack?

    // Keleseth
    SPELL_INVOCATION_KELESETH   = 70981,

    SPELL_SHADOW_LANCE          = 71405,
    SPELL_EMP_SHADOW_LANCE      = 71815,

    SPELL_SHADOW_RESONANCE      = 71943,
    SPELL_SHADOW_RESONANCE_AURA = 71911, // blizz used this for determining the distance, probably. (not used)
    SPELL_SHADOW_RESONANCE_BUFF = 71822,

    SPELL_SHADOW_PRISON         = 73001,

    // Taldaram
    SPELL_INVOCATION_TALDARAM   = 70982,

    SPELL_GLITTERING_SPARKS     = 71807,

    SPELL_BALL_FLAMES_VISUAL    = 71706,
    SPELL_CONJURE_FLAME         = 71718, // dummy effect, must trigger spell summoning the sphere
    SPELL_CONJURE_EMP_FLAME     = 72040,
    SPELL_FLAMES                = 71393, // cast on the impact
    SPELL_BALL_FLAMES_PERIODIC  = 71709, // used on heroic, triggers empowered flare which reduces dmg of flames on impact
    SPELL_BALL_FLAMES_TRIGGERED = 71708, // this spell must reduce stack of 71756
    SPELL_FLAMES_BUFF           = 71756,
};

// talks
enum
{
    SAY_COUNCIL_INTRO_1         = -1631101,                 // Intro by Bloodqueen
    SAY_COUNCIL_INTRO_2         = -1631102,

    SAY_KELESETH_INVOCATION     = -1631103,
    SAY_KELESETH_SPECIAL        = -1631104,
    SAY_KELESETH_SLAY_1         = -1631105,
    SAY_KELESETH_SLAY_2         = -1631106,
    SAY_KELESETH_BERSERK        = -1631107,
    SAY_KELESETH_DEATH          = -1631108,

    SAY_TALDARAM_INVOCATION     = -1631109,
    SAY_TALDARAM_SPECIAL        = -1631110,
    SAY_TALDARAM_SLAY_1         = -1631111,
    SAY_TALDARAM_SLAY_2         = -1631112,
    SAY_TALDARAM_BERSERK        = -1631113,
    SAY_TALDARAM_DEATH          = -1631114,

    SAY_VALANAR_INVOCATION      = -1631115,
    SAY_VALANAR_SPECIAL         = -1631116,
    SAY_VALANAR_SLAY_1          = -1631117,
    SAY_VALANAR_SLAY_2          = -1631118,
    SAY_VALANAR_BERSERK         = -1631119,
    SAY_VALANAR_DEATH           = -1631120,
};


struct MANGOS_DLL_DECL boss_blood_queen_lanathel_introAI : public ScriptedAI
{
    boss_blood_queen_lanathel_introAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiEventTimer;
    uint32 m_uiEventPhase;
    bool m_bEventStarted;
    bool m_bEventEnded;

    void Reset()
    {
        m_bEventStarted = false;
        m_bEventEnded = false;
        m_uiEventPhase = 0;
        m_uiEventTimer = 0;
    }

    void AttackStart(Unit *who){}

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_bEventStarted || m_bEventEnded)
            return;

        if (pWho && pWho->IsWithinDistInMap(m_creature, 50.0f) &&
            ((pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster()) ||
            pWho->GetObjectGuid().IsPet()))
        {
            if (Creature *pTaldaram = m_pInstance->GetSingleCreatureFromStorage(NPC_TALDARAM))
                pTaldaram->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (Creature *pKeleseth = m_pInstance->GetSingleCreatureFromStorage(NPC_KELESETH))
                pKeleseth->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (Creature *pValanar = m_pInstance->GetSingleCreatureFromStorage(NPC_VALANAR))
                pValanar->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            m_bEventStarted = true;
        }
    }

    void NextStep(uint32 uiTimer)
    {
        ++m_uiEventPhase;
        m_uiEventTimer = uiTimer;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bEventEnded || !m_bEventStarted)
            return;

        if (m_uiEventTimer <= uiDiff)
        {
            switch (m_uiEventPhase)
            {
                case 0:
                    DoScriptText(SAY_COUNCIL_INTRO_1, m_creature);
                    NextStep(15000);
                    break;
                case 1:
                    DoScriptText(SAY_COUNCIL_INTRO_2, m_creature);
                    NextStep(5000);
                    break;
                case 2:
                    // raise princes
                    if (m_pInstance)
                    {
                        if (Creature *pTaldaram = m_pInstance->GetSingleCreatureFromStorage(NPC_TALDARAM))
                        {
                            pTaldaram->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pTaldaram->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                            pTaldaram->SetInCombatWithZone();
                        }
                        if (Creature *pKeleseth = m_pInstance->GetSingleCreatureFromStorage(NPC_KELESETH))
                        {
                            pKeleseth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pKeleseth->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                            pKeleseth->SetInCombatWithZone();
                        }
                        if (Creature *pValanar = m_pInstance->GetSingleCreatureFromStorage(NPC_VALANAR))
                        {
                            pValanar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pValanar->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                            pValanar->SetInCombatWithZone();
                        }
                    }
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_bEventEnded = true;
                    break;
                default:
                    break;
            }
        }
        else
            m_uiEventTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_blood_queen_lanathel_intro(Creature* pCreature)
{
    return new boss_blood_queen_lanathel_introAI(pCreature);
}

// Blood Orb Controller
struct MANGOS_DLL_DECL npc_blood_orb_controlAI : public ScriptedAI
{
    npc_blood_orb_controlAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiCheckTimer;
    uint32 m_uiLastResult;
    uint32 m_uiInvocationTimer;
    bool m_bIsInProgress;

    void Reset()
    {
        m_uiCheckTimer = 1000;
        m_bIsInProgress = false;
        m_uiInvocationTimer = 30000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsInProgress)
        {
            if (m_uiCheckTimer <= uiDiff)
            {
                if (m_pInstance)
                {
                    if (m_pInstance->GetData(TYPE_BLOOD_COUNCIL) == IN_PROGRESS)
                    {
                        m_bIsInProgress = true;
                        DoCastSpellIfCan(m_creature, SPELL_INVOCATION_VALANAR, CAST_TRIGGERED);
                        m_uiLastResult = 0;
                    }
                }

                m_uiCheckTimer = 1000;
            }
            else
                m_uiCheckTimer -= uiDiff;

            return;
        }
        else
        {
            if (m_uiCheckTimer <= uiDiff)
            {
                if (m_pInstance)
                {
                    if (m_pInstance->GetData(TYPE_BLOOD_COUNCIL) == FAIL)
                    {
                        Reset();
                        return;
                    }
                }

                m_uiCheckTimer = 1000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }

        // battle in progress
        // every 30 seconds cast Invocation of Blood on random prince
        if (m_uiInvocationTimer <= uiDiff)
        {
            uint32 uiResult = urand(0, 2);
            uiResult = uiResult == m_uiLastResult ? (uiResult + 1) % 3 : uiResult;
            m_uiLastResult = uiResult;

            switch (uiResult)
            {
                case 0:
                    DoCastSpellIfCan(m_creature, SPELL_INVOCATION_V_MOVE, CAST_TRIGGERED);
                    DoCastSpellIfCan(m_creature, SPELL_INVOCATION_VALANAR, CAST_TRIGGERED);
                    break;
                case 1:
                    DoCastSpellIfCan(m_creature, SPELL_INVOCATION_K_MOVE, CAST_TRIGGERED);
                    DoCastSpellIfCan(m_creature, SPELL_INVOCATION_KELESETH, CAST_TRIGGERED);
                    break;
                case 2:
                    DoCastSpellIfCan(m_creature, SPELL_INVOCATION_T_MOVE, CAST_TRIGGERED);
                    DoCastSpellIfCan(m_creature, SPELL_INVOCATION_TALDARAM, CAST_TRIGGERED);
                    break;
            }

            m_uiInvocationTimer = 30000;
        }
        else
            m_uiInvocationTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_blood_orb_control(Creature* pCreature)
{
    return new npc_blood_orb_controlAI(pCreature);
}

// base struct for Blood Prince Council
struct MANGOS_DLL_DECL base_blood_prince_council_bossAI : public base_icc_bossAI
{
    base_blood_prince_council_bossAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
        DoCastSpellIfCan(m_creature, SPELL_FEIGN_DEATH, CAST_TRIGGERED);
    }
    
    bool m_bIsEmpowered;
    bool m_bIsSaidSpecial; // 1st spell cast after being empowered is followed by special say
    uint32 m_uiEmpowermentFadeTimer;
    uint32 m_uiInvocationSpellEntry;
    int32 m_iSayInvocationEntry;
    uint32 m_uiBerserkTimer;
    uint32 m_uiSphereTimer;

    void Reset()
    {
        m_bIsEmpowered = false;
        m_bIsSaidSpecial = false;
        m_uiEmpowermentFadeTimer = 30000;
        m_uiSphereTimer = urand(5000, 15000);
        m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
        m_creature->SetHealth(1);
    }

    void Aggro(Unit *pWho)
    {
        m_creature->CallForHelp(30.0f);
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        if (!m_bIsEmpowered && pDealer->GetEntry() != NPC_BLOOD_ORB_CONTROL)
            uiDamage = 0;
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
        {
            if (m_creature->GetEntry() != NPC_VALANAR)
            {
                if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_VALANAR))
                {
                    if (pTmp->isAlive())
                    {
                        pTmp->RemoveAurasDueToSpell(SPELL_INVOCATION_VALANAR);
                        pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                    }
                }
            }

            if (m_creature->GetEntry() != NPC_KELESETH)
            {
                if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_KELESETH))
                {
                    if (pTmp->isAlive())
                    {
                        pTmp->RemoveAurasDueToSpell(SPELL_INVOCATION_KELESETH);
                        pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                    }
                }
            }
            
            if (m_creature->GetEntry() != NPC_TALDARAM)
            {
                if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_TALDARAM))
                {
                    if (pTmp->isAlive())
                    {
                        pTmp->RemoveAurasDueToSpell(SPELL_INVOCATION_TALDARAM);
                        pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                    }
                }
            }

            if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_LANATHEL_INTRO))
            {
                if (pTmp->isAlive())
                    pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_BLOOD_COUNCIL) != FAIL)
                m_pInstance->SetData(TYPE_BLOOD_COUNCIL, FAIL);
        }
    }
    
    void SpellHit(Unit *pCaster, const SpellEntry *pSpell)
    {
        if (pSpell->Id == m_uiInvocationSpellEntry)
        {
            m_creature->SetHealth(pCaster->GetHealth());
            m_bIsEmpowered = true;
            DoScriptText(m_iSayInvocationEntry, m_creature);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Invocation of Blood
        if (m_bIsEmpowered)
        {
            if (m_uiEmpowermentFadeTimer <= uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(m_uiInvocationSpellEntry);
                m_creature->SetHealth(1);
                m_bIsEmpowered = false;
                m_bIsSaidSpecial = false;
                m_uiEmpowermentFadeTimer = 30000;
            }
            else
                m_uiEmpowermentFadeTimer -= uiDiff;
        }

        // Berserk
        if (m_uiBerserkTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
        }
        else
            m_uiBerserkTimer -= uiDiff;
    }
};


// Valanar
struct MANGOS_DLL_DECL boss_valanar_iccAI : public base_blood_prince_council_bossAI
{
    boss_valanar_iccAI(Creature* pCreature) : base_blood_prince_council_bossAI(pCreature)
    {
        Reset();
        m_uiInvocationSpellEntry = SPELL_INVOCATION_VALANAR;
        m_iSayInvocationEntry = SAY_VALANAR_INVOCATION;
    }

    uint32 m_uiVortexTimer;

    void Reset()
    {
        base_blood_prince_council_bossAI::Reset();

        m_uiVortexTimer = urand(5000, 10000);
    }

    void Aggro(Unit *pWho)
    {
        base_blood_prince_council_bossAI::Aggro(pWho);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_BLOOD_COUNCIL, IN_PROGRESS);

            if (Creature *pOrb = m_pInstance->GetSingleCreatureFromStorage(NPC_BLOOD_ORB_CONTROL))
            {
                uint32 uiHealth = m_creature->GetMaxHealth();
                pOrb->Respawn();
                pOrb->SetMaxHealth(uiHealth);
                pOrb->SetHealth(uiHealth);
            }
        }
    }

    void KilledUnit(Unit *pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_VALANAR_SLAY_1 - urand(0, 1), m_creature);
    }

    void JustDied(Unit *pKiller)
    {
        base_blood_prince_council_bossAI::JustDied(pKiller);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLOOD_COUNCIL, DONE);

        DoScriptText(SAY_VALANAR_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        base_blood_prince_council_bossAI::UpdateAI(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Kinetic Bomb
        if (m_uiSphereTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB) == CAST_OK)
                m_uiSphereTimer = 20000;
        }
        else
            m_uiSphereTimer -= uiDiff;

        // Shock Vortex
        if (m_uiVortexTimer <= uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SHOCK_VORTEX, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsEmpowered ? SPELL_EMP_SHOCK_VORTEX : SPELL_SHOCK_VORTEX) == CAST_OK)
                {
                    m_uiVortexTimer = urand(18000, 22000);

                    if (m_bIsEmpowered && !m_bIsSaidSpecial)
                    {
                        DoScriptText(SAY_VALANAR_SPECIAL, m_creature);
                        m_bIsSaidSpecial = false;
                    }
                }
            }
        }
        else
            m_uiVortexTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_valanar_icc(Creature* pCreature)
{
    return new boss_valanar_iccAI(pCreature);
}


// Keleseth
struct MANGOS_DLL_DECL boss_keleseth_iccAI : public base_blood_prince_council_bossAI
{
    boss_keleseth_iccAI(Creature* pCreature) : base_blood_prince_council_bossAI(pCreature)
    {
        Reset();
        m_uiInvocationSpellEntry = SPELL_INVOCATION_KELESETH;
        m_iSayInvocationEntry = SAY_KELESETH_INVOCATION;
    }

    uint32 m_uiShadowLanceTimer;
    bool m_bHasCastShadowPrison;

    void Reset()
    {
        base_blood_prince_council_bossAI::Reset();

        m_bHasCastShadowPrison = false;
        m_uiShadowLanceTimer = urand(2000, 5000);
        m_uiSphereTimer = 4000;
    }

    void KilledUnit(Unit *pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_KELESETH_SLAY_1 - urand(0, 1), m_creature);
    }

    void JustDied(Unit *pKiller)
    {
        base_blood_prince_council_bossAI::JustDied(pKiller);

        DoScriptText(SAY_KELESETH_DEATH, m_creature);
    }

    void JustSummoned(Creature *pSummoned)
    {
        pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        base_blood_prince_council_bossAI::UpdateAI(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsHeroic)
        {
            if (!m_bHasCastShadowPrison)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_PRISON) == CAST_OK)
                    m_bHasCastShadowPrison = true;
            }
        }

        // Shadow Resonance
        if (m_uiSphereTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_RESONANCE) == CAST_OK)
                m_uiSphereTimer = 20000;
        }
        else
            m_uiSphereTimer -= uiDiff;

        // Shadow Lance
        if (m_uiShadowLanceTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsEmpowered ? SPELL_EMP_SHADOW_LANCE : SPELL_SHADOW_LANCE) == CAST_OK)
            {
                if (m_bIsEmpowered && !m_bIsSaidSpecial)
                {
                    DoScriptText(SAY_KELESETH_SPECIAL, m_creature);
                    m_bIsSaidSpecial = true;
                }

                m_uiShadowLanceTimer = urand(2000, 5000);
            }
        }
        else
            m_uiShadowLanceTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_keleseth_icc(Creature* pCreature)
{
    return new boss_keleseth_iccAI(pCreature);
}


// Taldaram
struct MANGOS_DLL_DECL boss_taldaram_iccAI : public base_blood_prince_council_bossAI
{
    boss_taldaram_iccAI(Creature* pCreature) : base_blood_prince_council_bossAI(pCreature)
    {
        Reset();
        m_uiInvocationSpellEntry = SPELL_INVOCATION_TALDARAM;
        m_iSayInvocationEntry = SAY_TALDARAM_INVOCATION;
    }

    uint32 m_uiSparksTimer;

    void Reset()
    {
        base_blood_prince_council_bossAI::Reset();

        m_uiSparksTimer = urand(8000, 15000);
    }

    void KilledUnit(Unit *pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_TALDARAM_SLAY_1 - urand(0, 1), m_creature);
    }

    void JustDied(Unit *pKiller)
    {
        base_blood_prince_council_bossAI::JustDied(pKiller);

        DoScriptText(SAY_TALDARAM_DEATH, m_creature);
    }

    void JustSummoned(Creature *pSummoned)
    {
        pSummoned->SetInCombatWithZone();
        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, 99999, SELECT_FLAG_IN_LOS|SELECT_FLAG_PLAYER))
        {
            pSummoned->AddThreat(pTarget, 1000000.0f, true);
            pSummoned->AI()->AttackStart(pTarget);
            if (m_bIsEmpowered)
                pSummoned->CastSpell(pSummoned, SPELL_BALL_FLAMES_PERIODIC, true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        base_blood_prince_council_bossAI::UpdateAI(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Conjure Flame
        if (m_uiSphereTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsEmpowered ? SPELL_CONJURE_EMP_FLAME : SPELL_CONJURE_FLAME) == CAST_OK)
                m_uiSphereTimer = 20000;
        }
        else
            m_uiSphereTimer -= uiDiff;

        // Glittering Sparks
        if (m_uiSparksTimer <= uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_GLITTERING_SPARKS, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_GLITTERING_SPARKS) == CAST_OK)
                    m_uiSparksTimer = 30000;
            }
        }
        else
            m_uiSparksTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_taldaram_icc(Creature* pCreature)
{
    return new boss_taldaram_iccAI(pCreature);
}

// Shock Vortex
struct MANGOS_DLL_DECL mob_shock_vortexAI : public ScriptedAI
{
    mob_shock_vortexAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiCastTimer;
    bool m_bHasCast;

    void Reset()
    {
        m_creature->SetInCombatWithZone();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_bHasCast = false;
        m_uiCastTimer = 4000;
        SetCombatMovement(false);
        m_creature->ForcedDespawn(20000);
        DoCastSpellIfCan(m_creature, SPELL_SHOCK_VORTEX_VISUAL, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bHasCast)
        {
            if (m_uiCastTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SHOCK_VORTEX_AURA, CAST_TRIGGERED);
                m_bHasCast = true;
            }
            else
                m_uiCastTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_shock_vortex(Creature* pCreature)
{
     return new mob_shock_vortexAI (pCreature);
};

// Dark Nucleus
struct MANGOS_DLL_DECL mob_dark_nucleusAI : public base_icc_bossAI
{
    mob_dark_nucleusAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    void Reset(){}

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        DoResetThreat();
        m_creature->AddThreat(pDealer, 100000.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_BLOOD_COUNCIL) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetDistance(m_creature->getVictim()) < 14.0f)
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_RESONANCE_BUFF);
    }
};

CreatureAI* GetAI_mob_dark_nucleus(Creature* pCreature)
{
     return new mob_dark_nucleusAI (pCreature);
};

// Ball of Flames
struct MANGOS_DLL_DECL mob_ball_of_flamesAI : public ScriptedAI
{
    mob_ball_of_flamesAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 7.0f, false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiStartTimer;
    bool m_bIsDespawned;
    bool m_bIsStarted;

    void Reset()
    {
        m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetLevitate(true);
        m_creature->SetDisplayId(26767);

        m_uiStartTimer = 3000;
        m_bIsDespawned = false;
        m_bIsStarted = false;

        DoCastSpellIfCan(m_creature, SPELL_BALL_FLAMES_VISUAL, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsDespawned)
            return;

        if (!m_creature->getVictim())
            return;

        if (m_bIsStarted)
        {
            if (m_creature->GetDistance(m_creature->getVictim()) < INTERACTION_DISTANCE)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FLAMES) == CAST_OK)
                {
                    m_creature->ForcedDespawn(2000);
                    m_creature->RemoveAllAuras();
                    m_bIsDespawned = true;
                    return;
                }
            }
        }
        else
        {
            if (m_uiStartTimer <= uiDiff)
            {
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_bIsStarted = true;
            }
            else
            {
                m_creature->CastSpell(m_creature, SPELL_FLAMES_BUFF, true);
                m_uiStartTimer -= uiDiff;
            }
        }
    }
};

CreatureAI* GetAI_mob_ball_of_flames(Creature* pCreature)
{
     return new mob_ball_of_flamesAI (pCreature);
};

// Kinetic Bomb
struct MANGOS_DLL_DECL mob_kinetic_bombAI : public ScriptedAI
{
    mob_kinetic_bombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroic = pCreature->GetMap()->GetDifficulty() > RAID_DIFFICULTY_25MAN_NORMAL;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bHasCast;
    bool m_bIsHeroic;
    bool m_bIsStarted;
    uint32 m_uiStartTimer;

    void Reset()
    {
        float x, y, z;

        m_bHasCast = false;
        m_bIsStarted = false;
        m_uiStartTimer = 3000;

        m_creature->SetSpeedRate(MOVE_WALK, 0.3f);

        m_creature->SetDisplayId(31095);

        DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB_VISUAL, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_UNSTABLE, CAST_TRIGGERED);

        m_creature->SetLevitate(true);
        m_creature->GetPosition(x, y, z);
        m_creature->NearTeleportTo(x, y, z + (m_bIsHeroic ? 10.0f : 15.0f), 0.0f);

        m_creature->GetMotionMaster()->MovePoint(0, x, y, z, false);
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        uiDamage = 0;

        if (!m_creature->HasAura(SPELL_KINETIC_BOMB_VISUAL))
            DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB_VISUAL, CAST_TRIGGERED);
    }

    void AttackStart(Unit *pWho){}
    void EnterEvadeMode(){}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsStarted)
        {
            if (m_uiStartTimer <= uiDiff)
            {
                m_bIsStarted = true;
            }
            else
                m_uiStartTimer -= uiDiff;

            return;
        }

        if (m_bHasCast)
            return;

        if (Unit *creator = m_creature->GetCreator())
        {
            if (m_creature->GetDistance(creator) < 1.0f)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB_DMG) == CAST_OK)
                {
                    m_bHasCast = true;
                    m_creature->ForcedDespawn(3000);
                }
            }
        }
    }
};

CreatureAI* GetAI_mob_kinetic_bomb(Creature* pCreature)
{
     return new mob_kinetic_bombAI (pCreature);
};


struct MANGOS_DLL_DECL mob_kinetic_bomb_targetAI : public ScriptedAI
{
    mob_kinetic_bomb_targetAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset()
    {
        m_creature->SetDisplayId(31095);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);

        DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB_TARGET, CAST_TRIGGERED);
    }

    void SummonedCreatureJustDied(Creature* summoned)
    {
         m_creature->ForcedDespawn();
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        uiDamage = 0;
    }

    void AttackStart(Unit *who){}
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_kinetic_bomb_target(Creature* pCreature)
{
     return new mob_kinetic_bomb_targetAI (pCreature);
};


void AddSC_blood_prince_council()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_taldaram_icc";
    newscript->GetAI = &GetAI_boss_taldaram_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_keleseth_icc";
    newscript->GetAI = &GetAI_boss_keleseth_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_valanar_icc";
    newscript->GetAI = &GetAI_boss_valanar_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel_intro";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel_intro;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_blood_orb_control";
    newscript->GetAI = &GetAI_npc_blood_orb_control;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_shock_vortex";
    newscript->GetAI = &GetAI_mob_shock_vortex;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_nucleus";
    newscript->GetAI = &GetAI_mob_dark_nucleus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ball_of_flames";
    newscript->GetAI = &GetAI_mob_ball_of_flames;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_kinetic_bomb";
    newscript->GetAI = &GetAI_mob_kinetic_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_kinetic_bomb_target";
    newscript->GetAI = &GetAI_mob_kinetic_bomb_target;
    newscript->RegisterSelf();
}
