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

#include <cssdk/common/const.h>

namespace cssdk
{
#ifdef REHLDS_FIXES
    /**
     * @brief N/D
    */
    constexpr auto COM_TOKEN_LEN = 2048;
#else
    /**
     * @brief N/D
    */
    constexpr auto COM_TOKEN_LEN = 1024;
#endif
    /**
     * @brief Don't allow overflow.
    */
    constexpr auto SIZEBUF_CHECK_OVERFLOW = 0;

    /**
     * @brief N/D
    */
    constexpr auto SIZEBUF_ALLOW_OVERFLOW = 1 << 0;

    /**
     * @brief N/D
    */
    constexpr auto SIZEBUF_OVERFLOWED = 1 << 1;

    /**
     * @brief N/D
    */
    constexpr auto MAX_NUM_ARGVS = 50;

    /**
     * @brief N/D
    */
    constexpr auto NUM_SAFE_ARGVS = 7;

    /**
     * @brief N/D
    */
    constexpr auto COM_COPY_CHUNK_SIZE = 1024;

    /**
     * @brief N/D
    */
    constexpr auto COM_MAX_CMD_LINE = 256;

    struct SizeBuf
    {
        /**
         * @brief N/D
        */
        const char* buffer_name{};

        /**
         * @brief N/D
        */
        uint16 flags{};

        /**
         * @brief N/D
        */
        byte* data{};

        /**
         * @brief N/D
        */
        int max_size{};

        /**
         * @brief N/D
        */
        int cur_size{};
    };

    struct DownloadTime
    {
        /**
         * @brief N/D
        */
        qboolean used{};

        /**
         * @brief N/D
        */
        float time{};

        /**
         * @brief N/D
        */
        int bytes_remaining{};
    };

    struct IncomingTransfer
    {
        /**
         * @brief N/D
        */
        qboolean done_registering{};

        /**
         * @brief N/D
        */
        int percent{};

        /**
         * @brief N/D
        */
        qboolean download_requested{};

        /**
         * @brief N/D
        */
        DownloadTime stats[8];

        /**
         * @brief N/D
        */
        int cur_stat{};

        /**
         * @brief N/D
        */
        int total_size{};

        /**
         * @brief N/D
        */
        int total_to_transfer{};

        /**
         * @brief N/D
        */
        int remaining_to_transfer{};

        /**
         * @brief N/D
        */
        float last_status_update{};

        /**
         * @brief N/D
        */
        qboolean custom{};
    };
}