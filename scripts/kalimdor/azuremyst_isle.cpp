/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2011 - 2012 MangosR2 <http://github.com/mangosR2/>
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
SDName: Azuremyst_Isle
SD%Complete: 75
SDComment: Quest support: 9283, 9528, 9537, Injured Draenei cosmetic only
SDCategory: Azuremyst Isle
EndScriptData */

/* ContentData
npc_draenei_survivor
npc_engineer_spark_overgrind
npc_injured_draenei
npc_magwin
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include <cmath>

/*######
## npc_draenei_survivor
######*/

enum
{
    SAY_HEAL1           = -1000176,
    SAY_HEAL2           = -1000177,
    SAY_HEAL3           = -1000178,
    SAY_HEAL4           = -1000179,
    SAY_HELP1           = -1000180,
    SAY_HELP2           = -1000181,
    SAY_HELP3           = -1000182,
    SAY_HELP4           = -1000183,

    SPELL_IRRIDATION    = 35046,
    SPELL_STUNNED       = 28630
};

struct MANGOS_DLL_DECL npc_draenei_survivorAI : public ScriptedAI
{
    npc_draenei_survivorAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ObjectGuid m_casterGuid;

    uint32 m_uiSayThanksTimer;
    uint32 m_uiRunAwayTimer;
    uint32 m_uiSayHelpTimer;

    bool m_bCanSayHelp;

    void Reset()
    {
        m_casterGuid.Clear();

        m_uiSayThanksTimer = 0;
        m_uiRunAwayTimer = 0;
        m_uiSayHelpTimer = 10000;

        m_bCanSayHelp = true;

        m_creature->CastSpell(m_creature, SPELL_IRRIDATION, true);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.1));
        m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_bCanSayHelp && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsFriendlyTo(pWho) &&
            m_creature->IsWithinDistInMap(pWho, 25.0f))
        {
            //Random switch between 4 texts
            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_HELP1, m_creature, pWho); break;
                case 1: DoScriptText(SAY_HELP2, m_creature, pWho); break;
                case 2: DoScriptText(SAY_HELP3, m_creature, pWho); break;
                case 3: DoScriptText(SAY_HELP4, m_creature, pWho); break;
            }

            m_uiSayHelpTimer = 20000;
            m_bCanSayHelp = false;
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->SpellFamilyFlags.test<CF_ALL_GIFT_OF_THE_NAARU>())
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);

            m_creature->CastSpell(m_creature, SPELL_STUNNED, true);

            m_casterGuid = pCaster->GetObjectGuid();

            m_uiSayThanksTimer = 5000;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiSayThanksTimer)
        {
            if (m_uiSayThanksTimer <= uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_IRRIDATION);

                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_casterGuid))
                {
                    if (pPlayer->GetTypeId() != TYPEID_PLAYER)
                        return;

                    switch(urand(0, 3))
                    {
                        case 0: DoScriptText(SAY_HEAL1, m_creature, pPlayer); break;
                        case 1: DoScriptText(SAY_HEAL2, m_creature, pPlayer); break;
                        case 2: DoScriptText(SAY_HEAL3, m_creature, pPlayer); break;
                        case 3: DoScriptText(SAY_HEAL4, m_creature, pPlayer); break;
                    }

                    pPlayer->TalkedToCreature(m_creature->GetEntry(), m_creature->GetObjectGuid());
                }

                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MovePoint(0, -4115.053711f, -13754.831055f, 73.508949f);

                m_uiRunAwayTimer = 10000;
                m_uiSayThanksTimer = 0;
            }else m_uiSayThanksTimer -= uiDiff;

            return;
        }

        if (m_uiRunAwayTimer)
        {
            if (m_uiRunAwayTimer <= uiDiff)
                m_creature->ForcedDespawn();
            else
                m_uiRunAwayTimer -= uiDiff;

            return;
        }

        if (m_uiSayHelpTimer < uiDiff)
        {
            m_bCanSayHelp = true;
            m_uiSayHelpTimer = 20000;
        }else m_uiSayHelpTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_draenei_survivor(Creature* pCreature)
{
    return new npc_draenei_survivorAI(pCreature);
}

/*######
## npc_engineer_spark_overgrind
######*/

enum
{
    SAY_TEXT                = -1000184,
    EMOTE_SHELL             = -1000185,
    SAY_ATTACK              = -1000186,

    AREA_COVE               = 3579,
    AREA_ISLE               = 3639,
    QUEST_GNOMERCY          = 9537,
    FACTION_HOSTILE         = 14,
    SPELL_DYNAMITE          = 7978
};

#define GOSSIP_FIGHT        "Traitor! You will be brought to justice!"

struct MANGOS_DLL_DECL npc_engineer_spark_overgrindAI : public ScriptedAI
{
    npc_engineer_spark_overgrindAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormFaction = pCreature->getFaction();
        m_uiNpcFlags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);
        Reset();

        if (pCreature->GetAreaId() == AREA_COVE || pCreature->GetAreaId() == AREA_ISLE)
            m_bIsTreeEvent = true;
    }

    uint32 m_uiNpcFlags;
    uint32 m_uiNormFaction;

    uint32 m_uiDynamiteTimer;
    uint32 m_uiEmoteTimer;

    bool m_bIsTreeEvent;

    void Reset()
    {
        m_creature->setFaction(m_uiNormFaction);
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_uiNpcFlags);

        m_uiDynamiteTimer = 8000;
        m_uiEmoteTimer = urand(120000, 150000);

        m_bIsTreeEvent = false;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_ATTACK, m_creature, who);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->isInCombat() && !m_bIsTreeEvent)
        {
            if (m_uiEmoteTimer < diff)
            {
                DoScriptText(SAY_TEXT, m_creature);
                DoScriptText(EMOTE_SHELL, m_creature);
                m_uiEmoteTimer = urand(120000, 150000);
            }
            else m_uiEmoteTimer -= diff;
        }
        else if (m_bIsTreeEvent)
        {
            //nothing here yet
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDynamiteTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DYNAMITE);
            m_uiDynamiteTimer = 8000;
        }
        else m_uiDynamiteTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_engineer_spark_overgrind(Creature* pCreature)
{
    return new npc_engineer_spark_overgrindAI(pCreature);
}

bool GossipHello_npc_engineer_spark_overgrind(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_GNOMERCY) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_engineer_spark_overgrind(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->setFaction(FACTION_HOSTILE);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

/*######
## npc_injured_draenei
######*/

struct MANGOS_DLL_DECL npc_injured_draeneiAI : public ScriptedAI
{
    npc_injured_draeneiAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.15));
        switch(urand(0, 1))
        {
            case 0: m_creature->SetStandState(UNIT_STAND_STATE_SIT); break;
            case 1: m_creature->SetStandState(UNIT_STAND_STATE_SLEEP); break;
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        return;                                             //ignore everyone around them (won't aggro anything)
    }

    void UpdateAI(const uint32 uiDiff)
    {
        return;
    }

};
CreatureAI* GetAI_npc_injured_draenei(Creature* pCreature)
{
    return new npc_injured_draeneiAI(pCreature);
}

/*######
## npc_magwin
######*/

enum
{
    SAY_START              = -1000111,
    SAY_AGGRO              = -1000112,
    SAY_PROGRESS           = -1000113,
    SAY_END1               = -1000114,
    SAY_END2               = -1000115,
    EMOTE_HUG              = -1000116,
    QUEST_A_CRY_FOR_HELP   = 9528,
};

struct MANGOS_DLL_DECL npc_magwinAI : public npc_escortAI
{
    npc_magwinAI(Creature* pCreature) : npc_escortAI(pCreature) {Reset();}

    void WaypointReached(uint32 i)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch(i)
        {
            case 0:
                DoScriptText(SAY_START, m_creature, pPlayer);
                break;
            case 17:
                DoScriptText(SAY_PROGRESS, m_creature, pPlayer);
                break;
            case 28:
                DoScriptText(SAY_END1, m_creature, pPlayer);
                break;
            case 29:
                DoScriptText(EMOTE_HUG, m_creature, pPlayer);
                DoScriptText(SAY_END2, m_creature, pPlayer);
                pPlayer->GroupEventHappens(QUEST_A_CRY_FOR_HELP, m_creature);
                break;
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature, pWho);
    }

    void Reset() { }
};

bool QuestAccept_npc_magwin(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_A_CRY_FOR_HELP)
    {
        pCreature->setFaction(10);

        if (npc_magwinAI* pEscortAI = dynamic_cast<npc_magwinAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest);
    }
    return true;
}

CreatureAI* GetAI_npc_magwinAI(Creature* pCreature)
{
    return new npc_magwinAI(pCreature);
}

/*######
## boss_prophet_velen
######*/

enum
{
    SPELL_HOLY_BLAST                = 59700,
    SPELL_HOLY_NOVA                 = 59701,
    SPELL_HOLY_SMITE                = 59703,
    //SPELL_PRAYER_OF_HEALING         = 59698, //on friendly
    SPELL_STAFF_STRIKE              = 33542,
};

struct MANGOS_DLL_DECL boss_prophet_velenAI : public ScriptedAI
{
    boss_prophet_velenAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiHolyBlastTimer;
    uint32 m_uiHolyNovaTimer;
    uint32 m_uiHolySmiteTimer;
    uint32 m_uiStaffStrikeTimer;
    //uint32 m_uiPrayerOfHealingTimer;

    void Reset()
    {
        m_uiHolyBlastTimer      = 7000;
        m_uiHolyNovaTimer       = 12000;
        m_uiHolySmiteTimer      = 9000;
        m_uiStaffStrikeTimer    = 5000;
       //m_uiPrayerOfHealingTimer= 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHolyBlastTimer < uiDiff)
        {
            Unit* pTarget = (m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                DoCast(pTarget, SPELL_HOLY_BLAST);
            m_uiHolyBlastTimer = urand(7000, 11000);
        }
        else
            m_uiHolyBlastTimer -= uiDiff;

        if (m_uiHolyNovaTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_HOLY_NOVA);
            m_uiHolyNovaTimer = urand(12000, 17000);
        }
        else
            m_uiHolyNovaTimer -= uiDiff;

        if (m_uiHolySmiteTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_HOLY_SMITE);
            m_uiHolySmiteTimer = urand(8000, 12000);
        }
        else
            m_uiHolySmiteTimer -= uiDiff;

        if (m_uiStaffStrikeTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_STAFF_STRIKE);
            m_uiStaffStrikeTimer = urand(5000, 8000);
        }
        else
            m_uiStaffStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_prophet_velen(Creature* pCreature)
{
    return new boss_prophet_velenAI(pCreature);
}

void AddSC_azuremyst_isle()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_draenei_survivor";
    pNewScript->GetAI = &GetAI_npc_draenei_survivor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_engineer_spark_overgrind";
    pNewScript->GetAI = &GetAI_npc_engineer_spark_overgrind;
    pNewScript->pGossipHello =  &GossipHello_npc_engineer_spark_overgrind;
    pNewScript->pGossipSelect = &GossipSelect_npc_engineer_spark_overgrind;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_injured_draenei";
    pNewScript->GetAI = &GetAI_npc_injured_draenei;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_magwin";
    pNewScript->GetAI = &GetAI_npc_magwinAI;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_magwin;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_prophet_velen";
    pNewScript->GetAI = &GetAI_boss_prophet_velen;
    pNewScript->RegisterSelf();
}
