//
//  can.def.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/15/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//


/* This Definition File is to be #included in other namespaces.
 * Used correctly, it expands to either namespaces of namespaces of structs,
 *  or namespaces of "enum classes" of (enum-scoped) enum constants.
 */

/*
 * The List(x) MACRO shall yield a namespace or enum, called x
 * The Xbase MACRO shall yield the token `xbase` or a global variable declaration
 * The X(name, type, ...) MACRO shall begin with a correct separator symbol
 * The end MACRO shall yield a separator symbol if required 
 */

namespace bms {
	List(rx) {
		Xbase = 0x200
		X(trip, Float2, trip_code, module) = base
		X(reset_cc_batt, Empty)
		X(reset_cc_array, Empty)
		X(reset_cc_mppt1, Empty)
		X(reset_cc_mppt2, Empty)
		X(reset_cc_mppt3, Empty)
		X(reset_cc_Wh, Empty)
		X(reset_cc_all, Empty)
		X(max, Empty) = 0x20F
		end
	};
	List(tx) {
		Xbase = 0x210
		X(heartbeat, Status, bmsStr, reserved) = base
		X(error, Error)
		X(uptime, Double, seconds)
		X(last_reset, Trip, lastResetCode, reserved)
		X(batt_bypass, Module, module, reserved)
		X(current, Float2, array, battery)
		X(cc_array, Double, count)
		X(cc_batt, Double, count)
		X(cc_mppt1, Double, count)
		X(cc_mppt2, Double, count)
		X(cc_mppt3, Double, count)
		X(Wh_batt, Double, count)
		X(Wh_mppt1, Double, count)
		X(Wh_mppt2, Double, count)
		X(Wh_mppt3, Double, count)
		X(voltage, Module, module, voltage)
		X(owVoltage, Module, module, owVoltage)
		X(temp, Module, sensor, temp)
		X(trip, Trip, tripCode, module)
		X(last_trip, Trip, tripCode, module)
		X(trip_pt_current, Float2, low, high)
		X(trip_pt_voltage, Float2, low, high)
		X(trip_pt_temp, Float2, low, high)
		end
	};
}

namespace ws20 {
	List(rx) {
		Xbase = 0x400
		X(driver_controls_id, Status, drvId, serialNo) = base
		X(drive_cmd, Float2, motorVelocity, motorCurrent)
		X(power_cmd, Float2, reserved, busCurrent)
		X(reset_cmd, UInt2, unused1, unused2)
		end
	};
	List(tx) {
		Xbase = 0x500
		X(motor_id, Status, tritiumId, serialNo) = base
		X(motor_status_info, motor_Status)
		X(motor_bus, Float2, busVoltage, busCurrent)
		X(motor_velocity, Float2, motorVelocity, vehicleVelocity)
		X(motor_phase, Float2, phaseBCurrent, phaseACurrent)
		X(voltage_vector, Float2, voltageIm, voltageRe)
		X(current_vector, Float2, currentIm, currentRe)
		X(backemf, Float2, backEmfIm, backEmfRe)
		X(rail_15v_1pt65v, Float2, onePtSixtyFiveVRef, fifteenVPowerRail)
		X(rail_2pt5v_1pt2v, Float2, onePtTwoVSupply, twoPtFiveVSupply)
		X(fanspeed, Float2, fanDrive, fanRpm)
		X(sinks_temp, Float2, motorTemp, heatsinkTemp)
		X(cpu_airin_temp, Float2, processorTemp, airInletTemp)
		X(cap_airout_temp, Float2, capacitorTemp, airOutTemp)
		X(odom_bus_ah, Float2, odom, dcBusAmpHours)
		end
	};
}

namespace mppt {
	List(rx) {
		Xbase = 0b11100010000
		X(mppt1, Empty) = base
		X(mppt2, Empty) = base
		X(mppt3, Empty) = base
		end
	};
	List(tx) {
		Xbase = 0b11100010000
		X(mppt1, Empty) = base
		X(mppt2, Empty) = base
		X(mppt3, Empty) = base
		end
	};
}

namespace sw {
	List(rx) {
		Xbase = 0x300
		X(lights, sw_Lights) = base
		end
	};
	List(tx) {
		Xbase = 0x310
		X(heartbeat, Status, dcStr, reserved) = base
		X(error, Error)
		X(buttons, sw_Buttons)
		X(lights, sw_Lights)
		end
	};
}

namespace dc {
	List(rx) {
		Xbase = 0x110
		X(horn, UInt64, enabled) = base
		X(signals, UInt2, l, r)
		X(cruise, UInt64, enabled)
		X(cruise_velocity_current, Float2, velocity, current)
		end
	};
	List(tx) {
		Xbase = (int)ws20::rx::base
		end
	};
}
