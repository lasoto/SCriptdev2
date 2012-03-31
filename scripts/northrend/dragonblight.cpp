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
SDName: Dragonblight
SD%Complete: 100
SDComment: Quest support: 12166, 12261. Taxi paths Wyrmrest temple.
SDCategory: Dragonblight
EndScriptData */

/* ContentData
npc_afrasastrasz
npc_destructive_ward
npc_tariolstrasz
npc_torastrasza
npc_hourglass_of_eternity
EndContentData */

#include "precompiled.h"

/*######
## npc_afrasastrasz
######*/

enum
{
    TAXI_PATH_ID_MIDDLE_DOWN            = 882,
    TAXI_PATH_ID_MIDDLE_TOP             = 881
};

#define GOSSIP_ITEM_TAXI_MIDDLE_DOWN    "I would like to take a flight to the ground, Lord Of Afrasastrasz."
#define GOSSIP_ITEM_TAXI_MIDDLE_TOP     "My Lord, I must go to the upper floor of the temple."

bool GossipHello_npc_afrasastrasz(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_MIDDLE_DOWN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_MIDDLE_TOP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_afrasastrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_MIDDLE_DOWN);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_MIDDLE_TOP);
    }
    return true;
}

/*#####
# npc_destructive_ward
#####*/

enum
{
    SAY_WARD_POWERUP                    = -1000664,
    SAY_WARD_CHARGED                    = -1000665,

    SPELL_DESTRUCTIVE_PULSE             = 48733,
    SPELL_DESTRUCTIVE_BARRAGE           = 48734,
    SPELL_DESTRUCTIVE_WARD_POWERUP      = 48735,

    SPELL_SUMMON_SMOLDERING_SKELETON    = 48715,
    SPELL_SUMMON_SMOLDERING_CONSTRUCT   = 48718,
    SPELL_DESTRUCTIVE_WARD_KILL_CREDIT  = 52409,

    MAX_STACK                           = 1,
};

// Script is based on real event from you-know-where.
// Some sources show the event in a bit different way, for unknown reason.
// Devs decided to add it in the below way, until more details can be obtained.

// It will be only two power-up's, where other sources has a different count (2-4 stacks has been observed)
// Probably caused by either a change in a patch (bugfix?) or the powerup has a condition (some
// sources suggest this, but without any explanation about what this might be)

struct MANGOS_DLL_DECL npc_destructive_wardAI : public Scripted_NoMovementAI
{
    npc_destructive_wardAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_uiPowerTimer = 30000;
        m_uiStack = 0;
        m_uiSummonTimer = 2000;
        m_bCanPulse = false;
        m_bFirst = true;
        Reset();
    }

    uint32 m_uiPowerTimer;
    uint32 m_uiStack;
    uint32 m_uiSummonTimer;
    bool m_bFirst;
    bool m_bCanPulse;

    void Reset() { }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanPulse)
        {
            if (DoCastSpellIfCan(m_creature, m_uiStack > MAX_STACK ? SPELL_DESTRUCTIVE_BARRAGE : SPELL_DESTRUCTIVE_PULSE) == CAST_OK)
                m_bCanPulse = false;
        }

        if (m_uiSummonTimer)
        {
            if (m_uiSummonTimer <= uiDiff)
            {
                if (m_bFirst)
                    m_uiSummonTimer = 25000;
                else
                    m_uiSummonTimer = 0;

                switch(m_uiStack)
                {
                    case 0:
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_SKELETON, CAST_TRIGGERED);
                        break;
                    case 1:
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_CONSTRUCT, CAST_TRIGGERED);

                        if (m_bFirst)
                            break;

                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_CONSTRUCT, CAST_TRIGGERED);
                        break;
                    case 2:
                        if (m_bFirst)
                            break;

                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_SKELETON, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_SKELETON, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_CONSTRUCT, CAST_TRIGGERED);
                        break;
                }

                m_bFirst = !m_bFirst;
            }
            else
                m_uiSummonTimer -= uiDiff;
        }

        if (!m_uiPowerTimer)
            return;

        if (m_uiPowerTimer <= uiDiff)
        {
            if (m_uiStack > MAX_STACK)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_DESTRUCTIVE_WARD_KILL_CREDIT) == CAST_OK)
                {
                    DoScriptText(SAY_WARD_CHARGED, m_creature, m_creature->GetOwner());
                    m_uiPowerTimer = 0;
                    m_uiSummonTimer = 0;
                    m_bCanPulse = true;
                }
            }
            else if (DoCastSpellIfCan(m_creature, SPELL_DESTRUCTIVE_WARD_POWERUP) == CAST_OK)
            {
                DoScriptText(SAY_WARD_POWERUP, m_creature, m_creature->GetOwner());

                m_uiPowerTimer = 30000;
                m_uiSummonTimer = 2000;

                m_bFirst = true;
                m_bCanPulse = true;                         // pulse right after each charge

                ++m_uiStack;
            }
        }
        else
            m_uiPowerTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_destructive_ward(Creature* pCreature)
{
    return new npc_destructive_wardAI(pCreature);
}

/*######
## npc_tariolstrasz
######*/

enum
{
    QUEST_INFORM_QUEEN_A                = 12123,            //need to check if quests are required before gossip available
    QUEST_INFORM_QUEEN_H                = 12124,
    TAXI_PATH_ID_BOTTOM_TOP             = 878,
    TAXI_PATH_ID_BOTTOM_MIDDLE          = 883
};

#define GOSSIP_ITEM_TAXI_BOTTOM_TOP     "My Lord, I must go to the upper floor of the temple."
#define GOSSIP_ITEM_TAXI_BOTTOM_MIDDLE  "Can you spare a drake to travel to Lord Of Afrasastrasz, in the middle of the temple?"

bool GossipHello_npc_tariolstrasz(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_BOTTOM_TOP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_BOTTOM_MIDDLE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_tariolstrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_BOTTOM_TOP);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_BOTTOM_MIDDLE);
    }
    return true;
}

/*######
## npc_torastrasza
######*/

enum
{
    TAXI_PATH_ID_TOP_MIDDLE             = 880,
    TAXI_PATH_ID_TOP_BOTTOM             = 879
};

#define GOSSIP_ITEM_TAXI_TOP_MIDDLE     "I would like to see Lord Of Afrasastrasz, in the middle of the temple."
#define GOSSIP_ITEM_TAXI_TOP_BOTTOM     "Yes, Please. I would like to return to the ground floor of the temple."

bool GossipHello_npc_torastrasza(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_TOP_MIDDLE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_TOP_BOTTOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_torastrasza(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_TOP_MIDDLE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_TOP_BOTTOM);
    }
    return true;
}


/*###### 
## npc_hourglass_of_eternity 
######*/ 
/*Support for 'Future you' is currently missing*/  
enum 
{ 

    NPC_INFINITE_CHRONO_MAGUS    = 27898, 
    NPC_INFINITE_ASSAILANT       = 27896, 
    NPC_INFINITE_DESTROYER       = 27897, 
    NPC_INFINITE_TIMERENDER      = 27900, 
    QUEST_MYSTERY_OF_INFINITE    = 12470 
}; 

struct MANGOS_DLL_DECL npc_hourglassAI : public ScriptedAI 
{ 
    npc_hourglassAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); SetCombatMovement(false); } 

    uint32 uiWaveTimer; 
    uint32 uiWaveCounter;
 
    void Reset() 
    { 
        uiWaveTimer = 5000; 
        uiWaveCounter = 0; 
    }

    void JustSummoned(Creature* pSummoned)
    { 
        pSummoned->AI()->AttackStart(m_creature); 
    }

    void JustDied(Unit* pKiller) 
    { 
       if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetOwnerGuid())) 
       { 
           pPlayer->FailQuest(QUEST_MYSTERY_OF_INFINITE); 
       } 
    }

    void SummonWave() 
    { 
        switch(uiWaveCounter) 
        { 
           case 0: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   m_creature->SummonCreature(NPC_INFINITE_ASSAILANT, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   break; 
           case 1: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   break; 
           case 2: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   m_creature->SummonCreature(NPC_INFINITE_ASSAILANT, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   m_creature->SummonCreature(NPC_INFINITE_DESTROYER, m_creature->GetPositionX()+5,m_creature->GetPositionY()+5 ,m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   break; 
           case 3: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   m_creature->SummonCreature(NPC_INFINITE_ASSAILANT, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
                   m_creature->SummonCreature(NPC_INFINITE_DESTROYER, m_creature->GetPositionX()+5,m_creature->GetPositionY()+5 ,m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0); 
           case 4: m_creature->SummonCreature(NPC_INFINITE_TIMERENDER, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000); 
                   break; 
        } 
    }

    void UpdateAI(const uint32 uiDiff) 
    { 
           if (uiWaveTimer <= uiDiff) 
           { 
               if(uiWaveCounter<=4) 
               { 
                   SummonWave(); 
                   uiWaveTimer = 15000; 
                   uiWaveCounter++; 
               } 
               else m_creature->ForcedDespawn(); 
           } else uiWaveTimer -= uiDiff; 
    } 
};
 
CreatureAI* GetAI_npc_hourglass(Creature* pCreature) 
{ 
   return new npc_hourglassAI(pCreature); 
}

void AddSC_dragonblight()
{
   Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_afrasastrasz";
    pNewScript->pGossipHello = &GossipHello_npc_afrasastrasz;
    pNewScript->pGossipSelect = &GossipSelect_npc_afrasastrasz;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_destructive_ward";
    pNewScript->GetAI = &GetAI_npc_destructive_ward;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_tariolstrasz";
    pNewScript->pGossipHello = &GossipHello_npc_tariolstrasz;
    pNewScript->pGossipSelect = &GossipSelect_npc_tariolstrasz;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_torastrasza";
    pNewScript->pGossipHello = &GossipHello_npc_torastrasza;
    pNewScript->pGossipSelect = &GossipSelect_npc_torastrasza;
    pNewScript->RegisterSelf();

    pNewScript = new Script; 
    pNewScript->Name = "npc_hourglass"; 
    pNewScript->GetAI = &GetAI_npc_hourglass; 
    pNewScript->RegisterSelf();
}
