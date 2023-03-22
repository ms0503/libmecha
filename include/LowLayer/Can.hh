/*
 * Can.hh
 *
 *  Created on: 2019/06/29
 *      Author: youda, ms0503
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _LIBMECHA_CAN_HH_
#define _LIBMECHA_CAN_HH_

#include "Peripheral.hh"
#include "stm32f4xx_hal.h"
#include <array>
#include <cstdint>
#include <vector>

namespace LibMecha::LowLayer {
    /// CAN通信用低レイヤークラス
    class Can : public Peripheral {
    public:
        using Peripheral::Peripheral;

        /**
         * コンストラクタ
         * @param canHandle HALのCANハンドル
         */
        explicit Can(CAN_HandleTypeDef &canHandle);
        /**
         * CANのフィルターの設定
         * @param address CANアドレス
         */
        void setFilter(std::uint8_t address) const;
        /**
         * リモートへデータの送信
         * @param address リモートCANアドレス
         */
        void sendRemote(std::uint8_t address) const;
        /**
         * データの送信
         * @param address 送信先CANアドレス
         * @param sendData 送信データ
         * @param sendDataSize 送信データサイズ
         * @return 送信完了
         */
        template<std::size_t SIZE>
        inline bool send(std::uint8_t address, const std::uint8_t sendData[SIZE]) const {
            std::array<std::uint8_t, SIZE> sendData1 {};
            std::copy(sendData, sendData + SIZE, sendData1.begin());
            return send(address, sendData1);
        }
        /**
         * データの送信
         * @param address 送信先CANアドレス
         * @param sendData 送信データ
         * @param sendDataSize 送信データサイズ
         * @return 送信完了
         */
        template<std::size_t SIZE>
        bool send(std::uint8_t address, std::array<std::uint8_t, SIZE> sendData) const;
        /**
         * 初期化
         * @param address 自身のCANアドレス
         * @param receiveInterrupt 受信割り込み
         */
        void init(std::uint8_t address, std::uint32_t receiveInterrupt);
        /**
         * メッセージの受信
         * @param canRxFifo CAN受信FIFO
         * @return 受信データ
         */
        std::array<std::uint8_t, 8> getMessage(std::uint8_t canRxFifo) const;

    private:
        /// HALのCANハンドル
        CAN_HandleTypeDef &_hcan;
        /// 受信割り込み
        std::uint32_t _receiveInterrupt = CAN_IT_RX_FIFO0_MSG_PENDING;
    };
} // namespace LibMecha::LowLayer

#endif // _LIBMECHA_CAN_HH_
