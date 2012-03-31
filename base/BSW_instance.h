/* Copyright (C) 2009 - 2011 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef BSW_INSTANCE_H
#define BSW_INSTANCE_H
#define BSW_INSTANCE_VERSION 0.6.20
#include "sc_instance.h"

class MANGOS_DLL_DECL BSWScriptedInstance : public ScriptedInstance
{
    public:

        BSWScriptedInstance(Map* pMap);
        ~BSWScriptedInstance();

        //sends completed achievments to all players in instance
        void DoCompleteAchievement(uint32 uiAchievmentId);
        void DoOpenDoor(ObjectGuid guid);
        void DoCloseDoor(ObjectGuid guid);

        void DoOpenDoor(uint32 entry);
        void DoCloseDoor(uint32 entry);

        void SetCriteriaState(uint32 criteria_id, bool state = true, Player* player = NULL);
        bool GetCriteriaState(uint32 criteria_id, Player const* player = NULL);

        void   SetNextEvent(uint32 EventNum, uint32 creatureID, uint32 timer = 1000);
        uint32 GetEvent(uint32 creatureID);
        bool   GetEventTimer(uint32 creatureID, const uint32 diff);

    private:
        uint32 m_auiEvent;
        uint32 m_auiCreatureID;
        uint32 m_auiEventTimer;
        bool   m_auiEventLock;
        Map*   m_pMap;

        std::map<uint32, bool>            m_groupCriteriaMap;
        std::multimap<uint32, ObjectGuid> m_personalCriteriaMap;

};
#endif
