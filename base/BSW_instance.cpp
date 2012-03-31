/* Copyright (C) 2009 - 2011 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"
#include "BSW_instance.h"

BSWScriptedInstance::BSWScriptedInstance(Map* pMap) : ScriptedInstance(pMap)
{
    debug_log("BSW: Initialized BSWScriptedInstance structure for map %u difficulty %u",pMap->GetId(),pMap->GetDifficulty());
    m_auiEvent      = 0;
    m_auiEventTimer = 0;
    m_auiCreatureID = 0;
    m_auiEventLock  = false;
    m_pMap          = pMap;
};

BSWScriptedInstance::~BSWScriptedInstance()
{
     debug_log("BSW: Removing BSWScriptedInstance structure for map %u",m_pMap->GetId());
};

void BSWScriptedInstance::DoCompleteAchievement(uint32 uiAchievmentId)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->CompletedAchievement(uiAchievmentId);
        }
    }
    else
        debug_log("BSW: DoCompleteAchievement attempt set data, but no players in map.");
}

void BSWScriptedInstance::DoOpenDoor(ObjectGuid guid)
{
    if (guid.IsEmpty())
        return;

    GameObject* pGo = instance->GetGameObject(guid);

    if (pGo)
        pGo->SetGoState(GO_STATE_ACTIVE);
    else
        debug_log("BSW: DoOpenDoor attempt set data to object %u, but no this object", guid.GetCounter());
}

void BSWScriptedInstance::DoCloseDoor(ObjectGuid guid)
{
    if (guid.IsEmpty())
        return;

    GameObject* pGo = instance->GetGameObject(guid);

    if (pGo)
        pGo->SetGoState(GO_STATE_READY);
    else
        debug_log("BSW: DoCloseDoor attempt set data to object %u, but no this object", guid.GetCounter());
}

void BSWScriptedInstance::DoOpenDoor(uint32 entry)
{
    EntryGuidMap::iterator find = m_mGoEntryGuidStore.find(entry);
    if (find != m_mGoEntryGuidStore.end())
    {
        ObjectGuid guid = find->second;
        DoOpenDoor(guid);
    }
    else
        debug_log("BSW: Script call DoOpenDoor (by Entry), but no gameobject of entry %u was created yet, or it was not stored by script for map %u.", entry, instance->GetId());
}

void BSWScriptedInstance::DoCloseDoor(uint32 entry)
{
    EntryGuidMap::iterator find = m_mGoEntryGuidStore.find(entry);
    if (find != m_mGoEntryGuidStore.end())
    {
        ObjectGuid guid = find->second;
        DoCloseDoor(guid);
    }
    else
        debug_log("BSW: Script call DoCloseDoor (by Entry), but no gameobject of entry %u was created yet, or it was not stored by script for map %u.", entry, instance->GetId());
}

uint32 BSWScriptedInstance::GetEvent(uint32 creatureID)
{
    if (m_auiEventLock || m_auiCreatureID != creatureID)
    {
        return 0;
    }
    else
    {
        debug_log("BSW: GetEvent: send event %u to creature %u",m_auiEvent, creatureID);
        m_auiEventLock = true;
        return  m_auiEvent;
    }
}

void BSWScriptedInstance::SetNextEvent(uint32 EventNum, uint32 creatureID, uint32 timer)
{
    m_auiEvent      = EventNum;
    m_auiCreatureID = creatureID;
    m_auiEventTimer = timer;
    m_auiEventLock  = false;
    debug_log("BSW: SetNextEvent: setted event %u to creature %u, timer %u",m_auiEvent, creatureID, timer);
}

bool BSWScriptedInstance::GetEventTimer(uint32 creatureID, const uint32 diff)
{
    if (m_auiEvent == 0 || m_auiCreatureID != creatureID)
        return false;

    if (m_auiEventTimer <= diff)
    {
        m_auiEventTimer = 0;
        return true;
    }
    else
    {
        m_auiEventTimer -= diff;
        return false;
    }
}

void BSWScriptedInstance::SetCriteriaState(uint32 criteria_id, bool state, Player* player)
{
    if (!criteria_id)
        return;

    if (player && state)
        m_personalCriteriaMap.insert(std::make_pair(criteria_id, player->GetObjectGuid()));
    else
        m_groupCriteriaMap.insert(std::make_pair(criteria_id, state));

}

bool BSWScriptedInstance::GetCriteriaState(uint32 criteria_id, Player const* player)
{
    if (!criteria_id)
        return false;

    std::map<uint32, bool>::const_iterator itr = m_groupCriteriaMap.find(criteria_id);

    if (itr != m_groupCriteriaMap.end())
        if (itr->second)
            return true;

    if (player)
    {
        std::pair<std::multimap<uint32, ObjectGuid>::const_iterator, std::multimap<uint32, ObjectGuid>::const_iterator> bounds =
            m_personalCriteriaMap.equal_range(criteria_id);

        for(std::multimap<uint32, ObjectGuid>::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
        {
            if (itr->second == player->GetObjectGuid())
                return true;
        }

    }

    return false;
}
