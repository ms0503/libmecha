/*
 * MotorDriver.hh
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

#ifndef _LIBMECHA_MOTORDRIVER_HH_
#define _LIBMECHA_MOTORDRIVER_HH_

#include "Can.hh"
#include "Peripheral.hh"
#include "stm32f4xx_hal.h"
#include <array>
#include <cassert>
#include <cstring>

namespace LibMecha::LowLayer {
    /// メカトロ製モータードライバー用低レイヤークラス
    class MotorDriver : public Peripheral {
    public:
        using Peripheral::Peripheral;

        /// モータドライバーのコマンド
        enum class DriveCommand : std::uint8_t {
            /// Duty比の設定
            kDuty = 0x00,
            /// PID制御のターゲット回転数の設定
            kPID = 0x01,
            /// PID制御のKp値の設定
            kSetParamP = 0x02,
            /// PID制御のKi値の設定
            kSetParamI = 0x03,
            /// PID制御のKd値の設定
            kSetParamD = 0x04,
            /// PID制御の上限回転数の設定
            kSetParamLIMIT = 0x05,
            /// 不明
            kSetParamPPM = 0x06,
            /// 非常停止信号
            kEmergency = 0b1000'0000
        };

        /**
         * コンストラクタ
         * @param can Canクラスのインスタンス
         * @param address CANアドレス
         */
        explicit MotorDriver(Can &can, std::uint8_t address);
        /**
         * 初期化
         */
        void init() const;
        /**
         * PID制御の初期化
         * @param kp Kp値
         * @param ki Ki値
         * @param kd Kd値
         * @param maxRpm 上限回転数
         * @param kppm 不明
         */
        void pidInit(float kp, float ki, float kd, std::uint32_t maxRpm, std::uint32_t kppm) const;
        /**
         * Canクラスのインスタンスの取得
         * @return Canクラスのインスタンス
         */
        inline Can &getCan() const {
            return _can;
        }
        /**
         * ターゲット回転数の設定
         * @param targetRpm ターゲット回転数
         * @return 設定完了
         */
        bool setTargetRPM(std::int32_t targetRpm) const;
        /**
         * Duty比の設定
         * @param duty Duty比
         * @return 設定完了
         */
        bool setDuty(std::int32_t duty) const;
        /**
         * パラメータの設定
         * @param mode コマンド
         * @param fparamValue 値
         * @return 設定完了
         */
        inline bool setParameter(DriveCommand cmd, float fparamValue) const {
            union {
                float fparamValue1;
                std::uint32_t uparamValue;
            };
            fparamValue1 = fparamValue;

            return setParameter(cmd, uparamValue);
        }
        /**
         * パラメータの設定
         * @param mode コマンド
         * @param uparamValue 値
         * @return 設定完了
         */
        bool setParameter(DriveCommand cmd, std::uint32_t uparamValue) const;
        /**
         * 非常停止信号の送信
         * @return 送信完了
         */
        bool emergency() const;

    private:
        /// Canクラスのインスタンス
        Can &_can;
        /// CANアドレス
        std::uint8_t _address;

        /**
         * MD1枚のみのアップデート
         * @param cmd コマンド
         * @param sendData
         * @return
         */
        inline bool updateDataSend(DriveCommand cmd, const std::uint8_t sendData[4]) const {
            std::array<std::uint8_t, 4> sendData1 {};
            std::copy(sendData, sendData + 4, sendData1.begin());
            return updateDataSend(cmd, sendData1);
        }
        /**
         * MD1枚のみのアップデート
         * @param cmd コマンド
         * @param sendData
         * @return
         */
        bool updateDataSend(DriveCommand cmd, std::array<std::uint8_t, 4> sendData) const;
    };
} // namespace LibMecha::LowLayer

#endif // _LIBMECHA_MOTORDRIVER_HH_
