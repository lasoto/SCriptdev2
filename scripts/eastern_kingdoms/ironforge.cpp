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
SDName: Ironforge
SD%Complete: 100
SDComment: Quest support: 3702
SDCategory: Ironforge
EndScriptData */

/* ContentData
npc_royal_historian_archesonus
King Magni Bronzebeard
High Tinker Mekkatorque
EndContentData */

#include "precompiled.h"

/*######
## npc_royal_historian_archesonus
######*/

#define GOSSIP_ITEM_ROYAL   "I am ready to listen"
#define GOSSIP_ITEM_ROYAL_1 "That is tragic. How did this happen?"
#define GOSSIP_ITEM_ROYAL_2 "Interesting, continue please."
#define GOSSIP_ITEM_ROYAL_3 "Unbelievable! How dare they??"
#define GOSSIP_ITEM_ROYAL_4 "Of course I will help!"

bool GossipHello_npc_royal_historian_archesonus(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(3702) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(2235, pCreature->GetObjectGuid());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_royal_historian_archesonus(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(2236, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(2237, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(2238, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(2239, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(3702);
            break;
    }
    return true;
}

/*######
## King Magni Bronzebeard
######*/

enum
{
    SPELL_AVATAR                    = 19135,
    SPELL_KNOCK_AWAY                = 20686,
    SPELL_STORM_BOLT                = 20685,
    //SPELL_MOTRAL_STRIKE           = 13737,
    //SPELL_STRONG_CLEAVE           = 8255,
    //SPELL_RAMPAGE                 = 8285,
    //SPELL_CONCUSSION_BLOW         = 12809
};

struct MANGOS_DLL_DECL boss_king_magni_bronzebreardAI : public ScriptedAI
{
    boss_king_magni_bronzebreardAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiAvatarTimer;
    uint32 m_uiKnockAwayTimer;
    uint32 m_uiStormboltTimer;

    void Reset()
    {
        m_uiAvatarTimer     = 15000;
        m_uiKnockAwayTimer  = 8000;
        m_uiStormboltTimer  = 10000;
    }

    void Aggro(Unit *who)
    {
        m_creature->CallForHelp(100);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiAvatarTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_AVATAR);
            m_uiAvatarTimer = urand(15000, 12000);
        }
        else
            m_uiAvatarTimer -= uiDiff;

        if (m_uiKnockAwayTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_KNOCK_AWAY);
            m_uiKnockAwayTimer = urand(8000, 12000);
        }
        else
            m_uiKnockAwayTimer -= uiDiff;

        if (m_uiStormboltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_STORM_BOLT);
            m_uiStormboltTimer = 10000;
        }
        else
            m_uiStormboltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_king_magni_bronzebreard(Creature* pCreature)
{
    return new boss_king_magni_bronzebreardAI(pCreature);
}

/*######
## High Tinker Mekkatorque
######*/

enum
{
    SPELL_BOMB                      = 9143,
    SPELL_GOBLIN_DRAGON_GUN         = 22739,
    SPELL_SUPER_SHRINK_RAY          = 22742,

    //SPELL_IRON_GRENADE              = 4068,
    //SPELL_HI_EXP_BOMB               = 12543,
    //SPELL_MITHRIL_FRAG_BOMB         = 12421,
    //SPELL_BIG_IRON_BOMB             = 4069
};

struct MANGOS_DLL_DECL boss_high_tinker_mekkatorqueAI : public ScriptedAI
{
    boss_high_tinker_mekkatorqueAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiBombTimer;
    uint32 m_uiGoblinDragonGunTimer;
    uint32 m_uiSuperShrinkRayTimer;

    void Reset()
    {
        m_uiBombTimer               = 10000;
        m_uiGoblinDragonGunTimer    = 15000;
        m_uiSuperShrinkRayTimer     = 15000;
    }

    void Aggro(Unit *who)
    {
        m_creature->CallForHelp(100);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBombTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_BOMB);
            m_uiBombTimer = urand(8000, 15000);
        }
        else
            m_uiBombTimer -= uiDiff;

        if (m_uiGoblinDragonGunTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_GOBLIN_DRAGON_GUN);
            m_uiGoblinDragonGunTimer = urand(15000, 20000);
        }
        else
            m_uiGoblinDragonGunTimer -= uiDiff;

        if (m_uiSuperShrinkRayTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SUPER_SHRINK_RAY);
            m_uiSuperShrinkRayTimer = urand(15000, 20000);
        }
        else
            m_uiSuperShrinkRayTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_high_tinker_mekkatorque(Creature* pCreature)
{
    return new boss_high_tinker_mekkatorqueAI(pCreature);
}

void AddSC_ironforge()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_royal_historian_archesonus";
    pNewScript->pGossipHello =  &GossipHello_npc_royal_historian_archesonus;
    pNewScript->pGossipSelect = &GossipSelect_npc_royal_historian_archesonus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_king_magni_bronzebreard";
    pNewScript->GetAI = &GetAI_boss_king_magni_bronzebreard;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_high_tinker_mekkatorque";
    pNewScript->GetAI = &GetAI_boss_high_tinker_mekkatorque;
    pNewScript->RegisterSelf();
}
