/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2011 - 2012 MangosR2 <http://github.com/mangosR2/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_GUARDAI_H
#define SC_GUARDAI_H

enum
{
    GENERIC_CREATURE_COOLDOWN       = 5000,

    SAY_GUARD_SIL_AGGRO1            = -1000198,
    SAY_GUARD_SIL_AGGRO2            = -1000199,
    SAY_GUARD_SIL_AGGRO3            = -1000200,
};

enum eShattrathGuard
{
    SPELL_BANISHED_SHATTRATH_A      = 36642,
    SPELL_BANISHED_SHATTRATH_S      = 36671,
    SPELL_BANISH_TELEPORT           = 36643,
    SPELL_EXILE                     = 39533
};

enum SummonUnderAttackGuards
{
///-> values match faction values used by YTDB
    NPC_CENARION_INFANTRY           = 15184,        ///-> need to be added not using in intial developing  syill needs to be defined
    NPC_BLUFF_WATCHERS              = 3084,        ///-> Faction 105
    NPC_ORGRIMMAR_GRUNT             = 3296,        ///-> Faction 85
    NPC_DARNASSUS_SENTINEL          = 4262,        ///-> Faction 79
    NPC_STORMWIND_GUARD             = 68,          ///-> Faction 11
    NPC_UNDERCITY_GUARD             = 5624,        ///-> Faction 68
    NPC_IRONFORGE_GUARD             = 5595,        ///-> Faction 55
    NPC_EXODAR_PEACEKEEPER          = 16733,       ///-> Faction 1638
    NPC_SILVERMOON_CITYGUARD        = 16222,       ///-> Faction 1745
///-> Faction list for guard
    F_THUNDER_BLUFF                 = 105,
    F_ORGRIMMAR                     = 85,
    F_DARNASSUS                     = 79,
    F_STORMWIND                     = 11,
    F_UNDERCITY                     = 68,
    F_IRONFORGE                     = 55,
    F_EXODAR                        = 1638,
    F_SILVERMOON                    = 1745,
};

struct MANGOS_DLL_DECL guardAI : public ScriptedAI
{
    public:
        explicit guardAI(Creature* pCreature);
        ~guardAI() {}

        uint32 m_uiGlobalCooldown;                          // This variable acts like the global cooldown that players have (1.5 seconds)
        uint32 m_uiBuffTimer;                               // This variable keeps track of buffs
        uint32 m_uiZoneAttackMsgTimer;                      ///-> This keeps track of zone under attack message
        uint32 m_uiHelp;                                    ///-> help timer that aggroed guards

        void Reset();

        void Aggro(Unit *pWho);

        void JustDied(Unit *pKiller);

        void SummonGuardsHelpers();

        void UpdateAI(const uint32 uiDiff);

        // Commonly used for guards in main cities
        void DoReplyToTextEmote(uint32 uiTextEmote);
};

struct MANGOS_DLL_DECL guardAI_thunderbluff : public guardAI
{
    guardAI_thunderbluff(Creature* pCreature) : guardAI(pCreature) {}

    void ReceiveEmote(Player* pPlayer, uint32 uiTextEmote);
};

struct MANGOS_DLL_DECL guardAI_orgrimmar : public guardAI
{
    guardAI_orgrimmar(Creature* pCreature) : guardAI(pCreature) {}

    void ReceiveEmote(Player* pPlayer, uint32 uiTextEmote);
};

struct MANGOS_DLL_DECL guardAI_stormwind : public guardAI
{
    guardAI_stormwind(Creature* pCreature) : guardAI(pCreature) {}

    void ReceiveEmote(Player* pPlayer, uint32 uiTextEmote);
};

#endif
