/*
 *  Half Life 1 SDK Copyright(c) Valve Corp.
 *
 *  THIS DOCUMENT DESCRIBES A CONTRACT BETWEEN YOU AND VALVE CORPORATION ("Valve").
 *  PLEASE READ IT BEFORE DOWNLOADING OR USING THE HALF LIFE 1 SDK ("SDK"). BY
 *  DOWNLOADING AND/OR USING THE HALF LIFE 1 SDK YOU ACCEPT THIS LICENSE. IF YOU DO
 *  NOT AGREE TO THE TERMS OF THIS LICENSE PLEASE DON'T DOWNLOAD OR USE THE SDK.
 *
 *  You may, free of charge, download and use the SDK to develop a modified Valve
 *  game running on the Half-Life 1 engine. You may distribute your modified Valve
 *  game in source and object code form, but only for free. Terms of use for Valve
 *  games are found in the Steam Subscriber Agreement located here:
 *  http://store.steampowered.com/subscriber_agreement/
 *
 *  You may copy, modify, and distribute the SDK and any modifications you make to
 *  the SDK in source and object code form, but only for free. Any distribution of
 *  this SDK must include this LICENSE file.
 *
 *  Any distribution of the SDK or a substantial portion of the SDK must include
 *  the above copyright notice and the following:
 *
 *    DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 *    DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 *    DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 *    INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 *    NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *    LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 *    FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 *    (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 *    BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 *    LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 *    SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 *  If you would like to use the SDK for a commercial purpose, please contact Valve
 *  at sourceengine@valvesoftware.com.
 */

#pragma once

#include <cssdk/common/vector.h>
#include <cssdk/dll/entity_base.h>
#include <cssdk/dll/weapon_type.h>

namespace cssdk
{
    /**
     * @brief N/D
    */
    constexpr auto CS_ENTITY_API_INTERFACE_VERSION = "CSENTITY_API_INTERFACE_VERSION002";

    class CsEntity // NOLINT(cppcoreguidelines-special-member-functions)
    {
    public:
        /**
         * @brief N/D
        */
        virtual ~CsEntity() = default;

        /**
         * @brief N/D
        */
        virtual void FireBullets(int shots, Vector& src, Vector& dir_shooting, Vector& spread, float distance,
                                 BulletType bullet_type, int tracer_freq, int damage, EntityVars* attacker) = 0;

        /**
         * @brief N/D
        */
        virtual void FireBuckshots(unsigned long shots, Vector& src, Vector& dir_shooting, Vector& spread,
                                   float distance, int tracer_freq, int damage, EntityVars* attacker) = 0;

        /**
         * @brief N/D
        */
        virtual Vector FireBullets3(Vector& src, Vector& dir_shooting, float spread, float distance, int penetration,
                                    BulletType bullet_type, int damage, float range_modifier, EntityVars* attacker,
                                    bool pistol, int shared_rand) = 0;
        /**
         * @brief N/D
        */
        EntityBase* containing_entity{};

    private:
        virtual void FuncReserve1() = 0;
        virtual void FuncReserve2() = 0;
        virtual void FuncReserve3() = 0;
        virtual void FuncReserve4() = 0;
        virtual void FuncReserve5() = 0;
        virtual void FuncReserve6() = 0;
        virtual void FuncReserve7() = 0;
        virtual void FuncReserve8() = 0;
        virtual void FuncReserve9() = 0;
        virtual void FuncReserve10() = 0;
        virtual void FuncReserve11() = 0;
        virtual void FuncReserve12() = 0;
        virtual void FuncReserve13() = 0;
        virtual void FuncReserve14() = 0;
        virtual void FuncReserve15() = 0;
        virtual void FuncReserve16() = 0;
        virtual void FuncReserve17() = 0;
        virtual void FuncReserve18() = 0;
        virtual void FuncReserve19() = 0;
        virtual void FuncReserve20() = 0;
        virtual void FuncReserve21() = 0;
        virtual void FuncReserve22() = 0;
        virtual void FuncReserve23() = 0;
        virtual void FuncReserve24() = 0;
        virtual void FuncReserve25() = 0;
        virtual void FuncReserve26() = 0;
        virtual void FuncReserve27() = 0;
        virtual void FuncReserve28() = 0;
        virtual void FuncReserve29() = 0;
        virtual void FuncReserve30() = 0;
    };

    class CsDelay : public CsEntity
    {
    };

    class CsAnimating : public CsDelay
    {
    };

    class CsToggle : public CsAnimating
    {
    };

    class CsMonster : public CsToggle
    {
    };
}
