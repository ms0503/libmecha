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

#include "stm32f4xx_hal.h"
#include <array>
#include <cstdint>
#include <vector>

namespace LibMecha {
    inline namespace v2 {
        namespace LowLayer {
            /// CAN通信用低レイヤークラス
            class Can {
            public:
                /**
                 * コンストラクタ
                 * @param canHandle HALのCANハンドル
                 */
                explicit Can(const CAN_HandleTypeDef &canHandle);
                /**
                 * デストラクタ
                 */
                ~Can();
                /**
                 * CANのフィルターの設定
                 * @param address CANアドレス
                 */
                void setFilter(std::uint8_t address);
                /**
                 * リモートへデータの送信
                 * @param address リモートCANアドレス
                 */
                void sendRemote(std::uint8_t address);
                /**
                 * データの送信
                 * @param address 送信先CANアドレス
                 * @param sendData 送信データ
                 * @param sendDataSize 送信データサイズ
                 * @return 送信完了
                 */
                bool send(std::uint8_t address, std::uint8_t *sendData, std::size_t sendDataSize);
                /**
                 * データの送信
                 * @param address 送信先CANアドレス
                 * @param sendData 送信データ
                 * @param sendDataSize 送信データサイズ
                 * @return 送信完了
                 */
                inline bool send(std::uint8_t address, std::vector<uint8_t> sendData, std::size_t sendDataSize) {
                    return send(address, sendData.data(), sendDataSize);
                }
                /**
                 * 初期化
                 * @param address 自身のCANアドレス
                 * @param receiveInterrupt 受信割り込み
                 */
                void init(uint8_t address, uint32_t receiveInterrupt);
                /**
                 * メッセージの受信
                 * @param canRxFifo CAN受信FIFO
                 * @return 受信データ
                 */
                std::array<std::uint8_t, 8> getMessage(std::uint8_t canRxFifo);

            private:
                /// HALのCANハンドル
                CAN_HandleTypeDef _hcan{};
                /// 受信割り込み
                std::uint32_t _receiveInterrupt = CAN_IT_RX_FIFO0_MSG_PENDING;
            };
        }// namespace LowLayer
    }    // namespace v2
}// namespace LibMecha

#endif// _LIBMECHA_CAN_HH_
