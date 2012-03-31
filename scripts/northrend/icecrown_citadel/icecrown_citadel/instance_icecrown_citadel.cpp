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
SDName: instance_icecrown_spire
SD%Complete: 90%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"
#include "World.h"

static Locations SpawnLoc[]=
{
    {-446.788971f, 2003.362915f, 191.233948f},  // 0 Horde ship enter
    {-428.140503f, 2421.336914f, 191.233078f},  // 1 Alliance ship enter
};

instance_icecrown_spire::instance_icecrown_spire(Map* pMap) : ScriptedInstance(pMap)
{
    Difficulty = pMap->GetDifficulty();
    Initialize();
}

void instance_icecrown_spire::Initialize()
{
    for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        m_auiEncounter[i] = NOT_STARTED;

    m_auiEncounter[0] = 0;

    m_auiEvent = 0;
    m_auiEventTimer = 1000;
    m_uiCouncilInvocation = 0;
    m_uiDirection = 0;
    m_uiStinkystate = NOT_STARTED;
    m_uiPreciousstate = NOT_STARTED;

    switch (Difficulty)
    {
        case RAID_DIFFICULTY_10MAN_NORMAL:
            m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_10;
            m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_10;
            m_uiValithriaCache = GO_DREAMWALKER_CACHE_10;
            m_uiSaurfangCache = GO_SAURFANG_CACHE_10;
            break;
        case RAID_DIFFICULTY_10MAN_HEROIC:
            m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_10H;
            m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_10H;
            m_uiValithriaCache = GO_DREAMWALKER_CACHE_10_H;
            m_uiSaurfangCache = GO_SAURFANG_CACHE_10_H;
            break;
        case RAID_DIFFICULTY_25MAN_NORMAL:
            m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_25;
            m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_25;
            m_uiValithriaCache = GO_DREAMWALKER_CACHE_25;
            m_uiSaurfangCache = GO_SAURFANG_CACHE_25;
            break;
        case RAID_DIFFICULTY_25MAN_HEROIC:
            m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_25H;
            m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_25H;
            m_uiValithriaCache = GO_DREAMWALKER_CACHE_25_H;
            m_uiSaurfangCache = GO_SAURFANG_CACHE_25_H;
            break;
        default:
            break;
    }
}

bool instance_icecrown_spire::IsEncounterInProgress()
{
    for(uint8 i = 1; i < MAX_ENCOUNTERS-2 ; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_icecrown_spire::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_LORD_MARROWGAR:
        case NPC_LADY_DEATHWHISPER:
        case NPC_DEATHBRINGER_SAURFANG:
        case NPC_FESTERGUT:
        case NPC_ROTFACE:
        case NPC_PROFESSOR_PUTRICIDE:
        case NPC_TALDARAM:
        case NPC_VALANAR:
        case NPC_KELESETH:
        case NPC_LANATHEL:
        case NPC_LANATHEL_INTRO:
        case NPC_VALITHRIA:
        case NPC_VALITHRIA_QUEST:
        case NPC_SINDRAGOSA:
        case NPC_LICH_KING:
        case NPC_TIRION:
        case NPC_RIMEFANG:
        case NPC_SPINESTALKER:
        case NPC_STINKY:
        case NPC_PRECIOUS:
        case NPC_COMBAT_TRIGGER:
        case NPC_FROSTMOURNE_TRIGGER:
        case NPC_FROSTMOURNE_HOLDER:
        case NPC_BLOOD_ORB_CONTROL:
             m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
             break;
    }
}

void instance_icecrown_spire::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ICEWALL_1:
            if (m_auiEncounter[TYPE_MARROWGAR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_ICEWALL_2:
            if (m_auiEncounter[TYPE_MARROWGAR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_DEATHWHISPER_ELEVATOR:
        case GO_SAURFANG_DOOR:
            if (m_auiEncounter[TYPE_SAURFANG] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_ORANGE_PLAGUE:
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_GREEN_PLAGUE:
            if (m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SCIENTIST_DOOR_GREEN:
            if (m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SCIENTIST_DOOR_ORANGE:
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SCIENTIST_DOOR_COLLISION:
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE && m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SCIENTIST_DOOR:
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE && m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_CRIMSON_HALL_DOOR:
            if (m_auiEncounter[TYPE_BLOOD_COUNCIL] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_COUNCIL_DOOR_1:
            if (m_auiEncounter[TYPE_BLOOD_COUNCIL] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_COUNCIL_DOOR_2:
            if (m_auiEncounter[TYPE_BLOOD_COUNCIL] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_GREEN_DRAGON_DOOR_2:
            if (m_auiEncounter[TYPE_VALITHRIA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SINDRAGOSA_DOOR_1:
            if (m_auiEncounter[TYPE_VALITHRIA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SINDRAGOSA_DOOR_2:
            if (m_auiEncounter[TYPE_VALITHRIA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_SAURFANG_CACHE_10:
        case GO_SAURFANG_CACHE_25:
        case GO_SAURFANG_CACHE_10_H:
        case GO_SAURFANG_CACHE_25_H:
        case GO_GUNSHIP_ARMORY_A_10:
        case GO_GUNSHIP_ARMORY_A_25:
        case GO_GUNSHIP_ARMORY_A_10H:
        case GO_GUNSHIP_ARMORY_A_25H:
        case GO_GUNSHIP_ARMORY_H_10:
        case GO_GUNSHIP_ARMORY_H_25:
        case GO_GUNSHIP_ARMORY_H_10H:
        case GO_GUNSHIP_ARMORY_H_25H:
        case GO_DREAMWALKER_CACHE_10:
        case GO_DREAMWALKER_CACHE_25:
        case GO_DREAMWALKER_CACHE_10_H:
        case GO_DREAMWALKER_CACHE_25_H:
        case GO_ICESHARD_1:
        case GO_ICESHARD_2:
        case GO_ICESHARD_3:
        case GO_ICESHARD_4:
        case GO_FROSTY_WIND:
        case GO_FROSTY_EDGE:
        case GO_SNOW_EDGE:
        case GO_ARTHAS_PLATFORM:
        case GO_ARTHAS_PRECIPICE:
        case GO_GAS_RELEASE_VALVE:
        case GO_MARROWGAR_DOOR:
        case GO_BLOODPRINCE_DOOR:
        case GO_ICECROWN_GRATE:
        case GO_SINDRAGOSA_ENTRANCE:
        case GO_VALITHRIA_DOOR_1:
        case GO_VALITHRIA_DOOR_2:
        case GO_VALITHRIA_DOOR_3:
        case GO_VALITHRIA_DOOR_4:
        case GO_FROSTWING_DOOR:
        case GO_GREEN_DRAGON_DOOR_1:
        case GO_BLOODWING_DOOR:
        case GO_ORATORY_DOOR:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
    }
}

void instance_icecrown_spire::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_TELEPORT:
            break;
        case TYPE_MARROWGAR:
            m_auiEncounter[TYPE_MARROWGAR] = uiData;

            DoUseDoorOrButton(GO_MARROWGAR_DOOR);

            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ICEWALL_1);
                DoUseDoorOrButton(GO_ICEWALL_2);
                // door to Deathwhisper are already open
                //DoUseDoorOrButton(GO_ORATORY_DOOR);
            }
            break;
         case TYPE_DEATHWHISPER:
            m_auiEncounter[TYPE_DEATHWHISPER] = uiData;

            DoUseDoorOrButton(GO_ORATORY_DOOR);

            if (uiData == DONE)
            {
                if (GameObject* pGO = GetSingleGameObjectFromStorage(GO_DEATHWHISPER_ELEVATOR))
                {
                      pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                      pGO->SetGoState(GO_STATE_READY);
                }
            }
            break;
         case TYPE_FLIGHT_WAR:
            m_auiEncounter[3] = uiData;
            break;
         case TYPE_SAURFANG:
            m_auiEncounter[TYPE_SAURFANG] = uiData;

            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_SAURFANG_DOOR);

                if (GameObject* pChest = GetSingleGameObjectFromStorage(m_uiSaurfangCache))
                {
                    if (pChest && !pChest->isSpawned())
                        pChest->SetRespawnTime(7*DAY);
                }
            }
            break;
         case TYPE_FESTERGUT:
            m_auiEncounter[TYPE_FESTERGUT] = uiData;

            DoUseDoorOrButton(GO_ORANGE_PLAGUE);

            if (uiData == DONE)
            {
                if (m_auiEncounter[TYPE_ROTFACE] == DONE)
                {
                    DoUseDoorOrButton(GO_SCIENTIST_DOOR_COLLISION);
                    DoUseDoorOrButton(GO_SCIENTIST_DOOR_ORANGE);
                }
            }
            break;
         case TYPE_ROTFACE:
            m_auiEncounter[TYPE_ROTFACE] = uiData;

            DoUseDoorOrButton(GO_GREEN_PLAGUE);

            if (uiData == DONE)
            {
                if (m_auiEncounter[TYPE_FESTERGUT] == DONE)
                {
                    DoUseDoorOrButton(GO_SCIENTIST_DOOR_GREEN);
                    DoUseDoorOrButton(GO_SCIENTIST_DOOR_COLLISION);
                }
            }
            break;
         case TYPE_PUTRICIDE:
            m_auiEncounter[TYPE_PUTRICIDE] = uiData;

            if (uiData == DONE)
            {
                if (m_auiEncounter[TYPE_SINDRAGOSA] == DONE &&
                    m_auiEncounter[TYPE_LANATHEL] == DONE)
                {
                    m_auiEncounter[TYPE_KINGS_OF_ICC] = DONE;
                }
            }

            {
                // Proff sometimes does't trigger door, so let's check it explicitly
                GameObject* pDoor = GetSingleGameObjectFromStorage(GO_SCIENTIST_DOOR);
                if (pDoor)
                {
                    switch (uiData)
                    {
                        case IN_PROGRESS:
                        case SPECIAL:
                        {
                            // Close door if it's open
                            if (pDoor->getLootState() != GO_ACTIVATED)
                                DoUseDoorOrButton(GO_SCIENTIST_DOOR);
                            break;
                        }
                        case NOT_STARTED:
                        case FAIL:
                        case DONE:
                        {
                            // Open door if it's closed
                            if (pDoor->getLootState() != GO_READY)
                                DoUseDoorOrButton(GO_SCIENTIST_DOOR);
                            break;
                        }
                        default: break;
                    }
                }
            }

            break;
         case TYPE_BLOOD_COUNCIL:
            m_auiEncounter[TYPE_BLOOD_COUNCIL] = uiData;

            DoUseDoorOrButton(GO_CRIMSON_HALL_DOOR);

            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_COUNCIL_DOOR_1);
                DoUseDoorOrButton(GO_COUNCIL_DOOR_2);
            }
            break;
         case TYPE_LANATHEL:
            m_auiEncounter[TYPE_LANATHEL] = uiData;

            DoUseDoorOrButton(GO_BLOODPRINCE_DOOR);

            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ICECROWN_GRATE);

                if (m_auiEncounter[TYPE_PUTRICIDE] == DONE &&
                    m_auiEncounter[TYPE_SINDRAGOSA] == DONE)
                {
                    m_auiEncounter[TYPE_KINGS_OF_ICC] = DONE;
                }
            }
            break;
         case TYPE_VALITHRIA:
            m_auiEncounter[TYPE_VALITHRIA] = uiData;

            DoUseDoorOrButton(GO_GREEN_DRAGON_DOOR_1);
            DoUseDoorOrButton(GO_VALITHRIA_DOOR_1);
            DoUseDoorOrButton(GO_VALITHRIA_DOOR_2);

            if (instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL ||
                instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                DoUseDoorOrButton(GO_VALITHRIA_DOOR_3);
                DoUseDoorOrButton(GO_VALITHRIA_DOOR_4);
            }

            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_GREEN_DRAGON_DOOR_2);

                DoUseDoorOrButton(GO_SINDRAGOSA_DOOR_1);
                DoUseDoorOrButton(GO_SINDRAGOSA_DOOR_2);

                if (GameObject* pChest = GetSingleGameObjectFromStorage( m_uiValithriaCache))
                {
                    if (pChest && !pChest->isSpawned())
                        pChest->SetRespawnTime(7*DAY);
                }
            }

            break;
         case TYPE_SINDRAGOSA:
            m_auiEncounter[TYPE_SINDRAGOSA] = uiData;

            DoUseDoorOrButton(GO_SINDRAGOSA_ENTRANCE);

            if (uiData == DONE)
            {
                if (m_auiEncounter[TYPE_PUTRICIDE] == DONE &&
                    m_auiEncounter[TYPE_LANATHEL] == DONE)
                {
                    m_auiEncounter[TYPE_KINGS_OF_ICC] = DONE;
                }
            }
            break;
         case TYPE_LICH_KING:
            m_auiEncounter[TYPE_LICH_KING] = uiData;
            break;
         case TYPE_FROSTMOURNE_ROOM:
             m_auiEncounter[TYPE_FROSTMOURNE_ROOM] = uiData;
             break;
         case TYPE_ICECROWN_QUESTS:
            m_auiEncounter[TYPE_ICECROWN_QUESTS] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            saveStream << m_auiEncounter[i] << " ";

        strSaveData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_icecrown_spire::GetData(uint32 uiType)
{
    switch(uiType)
    {
         case TYPE_TELEPORT:
         case TYPE_MARROWGAR:
         case TYPE_DEATHWHISPER:
         case TYPE_FLIGHT_WAR:
         case TYPE_SAURFANG:
         case TYPE_FESTERGUT:
         case TYPE_ROTFACE:
         case TYPE_PUTRICIDE:
         case TYPE_BLOOD_COUNCIL:
         case TYPE_LANATHEL:
         case TYPE_VALITHRIA:
         case TYPE_SINDRAGOSA:
         case TYPE_KINGS_OF_ICC:
         case TYPE_LICH_KING:
         case TYPE_FROSTMOURNE_ROOM:
         case TYPE_ICECROWN_QUESTS:
             return m_auiEncounter[uiType];
         default:
             return 0;
    }
}

void instance_icecrown_spire::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);

    for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
    {
        loadStream >> m_auiEncounter[i];

        if (m_auiEncounter[i] == IN_PROGRESS && i >= 1)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_icecrown_spire(Map* pMap)
{
    return new instance_icecrown_spire(pMap);
}


void AddSC_instance_icecrown_spire()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_icecrown_spire";
    pNewScript->GetInstanceData = &GetInstanceData_instance_icecrown_spire;
    pNewScript->RegisterSelf();
}
