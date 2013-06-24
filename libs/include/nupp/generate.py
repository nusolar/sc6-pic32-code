#!/usr/bin/env python3

Empty = ""
Module = "uint32_t %s; float %s;"
Double = "double %s;"
Float2 = "float %s, %s;"
UInt64 = "uint64_t %s;"
UInt2 = "uint32_t %s, %s;"
Trip = "int32_t %s; uint32_t %s;"
Error = "char msg[8];"
Status = "char %s[4]; uint32_t %s;"
motor_Status = """
	uint16_t limitFlags;
	uint16_t errorFlags;
	uint16_t activeMotor;
	uint16_t reserved __attribute__ ((__packed__));"""
sw_Lights = """
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    radio           :1;
	unsigned    yes             :1;
	unsigned    hazard          :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_up       :1;
	unsigned    maybe           :1;
	unsigned    no              :1;
	unsigned    horn            :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :20;"""
sw_Buttons_old = """
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    hazard          :1;
	unsigned    horn            :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :21;"""
sw_Buttons = """
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :25;"""


class X(object):
	"""A prototype class of CAN frame"""
	struct = """
		struct %(name)s: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					%(members)s
				} contents;
			} frame;
			static const uint32_t _id = %(pre_id)s;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE %(name)s(): frame{0} {}
			ALWAYSINLINE %(name)s(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE %(name)s(const Packet& p): frame{p.data()} {}
		};\n"""
	def __init__(self, name, T, *args):
		self.name = name
		self.string = self.struct % {'name': name, 'members': T % tuple(args), 'pre_id': '%(id)s'}
		self.id = 0
	def __add__(self, other):
		self.id = other
		return self
	def enum_name(self):
		return self.name + '_k' + (' = ' + hex(self.id)) if self.id else ''
	def __str__(self):
		return self.string % {'id': hex(self.id)}


namespace = ' namespace %s {\n%s}\n'
enum = ' enum %s {\n%s};\n'

class EnumNS:
	def __init__(self):
		for i in range(len(self.frames)):
			if not self.frames[i].id:
				self.frames[i].id = self.frames[i-1].id + 1
	def __str__(self):
		contents = ''.join([str(x) for x in self.frames])
		contents += enum % ('addrs', ',\n'.join([x.enum_name() for x in self.frames]))
		return namespace % (type(self).__name__, contents)

class Namespace:
	def __str__(self):
		contents = str(self.rx()) + str(self.tx())
		return namespace % (type(self).__name__, contents)


class bms(Namespace):
	class rx(EnumNS):
		base = 0x200
		frames = (
			X('trip', Trip, 'trip_code', 'module') + base,
			X('reset_cc_batt', Empty),
			X('reset_cc_array', Empty),
			X('reset_cc_mppt1', Empty),
			X('reset_cc_mppt2', Empty),
			X('reset_cc_mppt3', Empty),
			X('reset_cc_Wh', Empty),
			X('reset_cc_all', Empty),
			X('max', Empty) + 0x20F)
	class tx(EnumNS):
		base = 0x210
		frames = (
			X('heartbeat', Status, 'bms_str', 'reserved') + base,
			X('error', Error),
			X('uptime', Double, 'seconds'),
			X('last_reset', Trip, 'last_reset_code', 'reserved'),
			X('batt_bypass', Module, 'module', 'reserved'),
			X('current', Float2, 'array', 'battery'),
			X('cc_array', Double, 'count'),
			X('cc_batt', Double, 'count'),
			X('cc_mppt1', Double, 'count'),
			X('cc_mppt2', Double, 'count'),
			X('cc_mppt3', Double, 'count'),
			X('Wh_batt', Double, 'count'),
			X('Wh_mppt1', Double, 'count'),
			X('Wh_mppt2', Double, 'count'),
			X('Wh_mppt3', Double, 'count'),
			X('voltage', Module, 'module', 'voltage'),
			X('owVoltage', Module, 'module', 'ow_voltage'),
			X('temp', Module, 'sensor', 'temp'),
			X('trip', Trip, 'trip_code', 'module'),
			X('last_trip', Trip, 'trip_code', 'module'),
			X('trip_pt_current', Float2, 'low', 'high'),
			X('trip_pt_voltage', Float2, 'low', 'high'),
			X('trip_pt_temp', Float2, 'low', 'high'))
class ws20(Namespace):
	class rx(EnumNS):
		base = 0x500
		frames = (
			X('driver_controls_id', Status, 'drvId', 'serialNo') + base,
			X('drive_cmd', Float2, 'motorVelocity', 'motorCurrent'),
			X('power_cmd', Float2, 'reserved', 'busCurrent'),
			X('reset_cmd', UInt2, 'unused1', 'unused2'))
	class tx(EnumNS):
		base = 0x400
		frames = (
			X('motor_id', Status, 'tritiumId', 'serialNo') + base,
			X('motor_status_info', motor_Status),
			X('motor_bus', Float2, 'busVoltage', 'busCurrent'),
			X('motor_velocity', Float2, 'motorVelocity', 'vehicleVelocity'),
			X('motor_phase', Float2, 'phaseBCurrent', 'phaseACurrent'),
			X('voltage_vector', Float2, 'voltageIm', 'voltageRe'),
			X('current_vector', Float2, 'currentIm', 'currentRe'),
			X('backemf', Float2, 'backEmfIm', 'backEmfRe'),
			X('rail_15v_1pt65v', Float2, 'onePtSixtyFiveVRef', 'fifteenVPowerRail'),
			X('rail_2pt5v_1pt2v', Float2, 'onePtTwoVSupply', 'twoPtFiveVSupply'),
			X('fanspeed', Float2, 'fanDrive', 'fanRpm'),
			X('sinks_temp', Float2, 'motorTemp', 'heatsinkTemp'),
			X('cpu_airin_temp', Float2, 'processorTemp', 'airInletTemp'),
			X('cap_airout_temp', Float2, 'capacitorTemp', 'airOutTemp'),
			X('odom_bus_ah', Float2, 'odom', 'dcBusAmpHours'))
class mppt(Namespace):
	class rx(EnumNS):
		base = 0b11100010000
		frames = (
			X('mppt1', Empty) + base,
			X('mppt2', Empty) + base,
			X('mppt3', Empty) + base)
	class tx(EnumNS):
		base = 0b11100010000
		frames = (
			X('mppt1', Empty) + base,
			X('mppt2', Empty) + base,
			X('mppt3', Empty) + base)
class sw(Namespace):
	class rx(EnumNS):
		base = 0x300
		frames = (
			X('buttons', sw_Buttons) + base,
			X('lights', sw_Lights))
	class tx(EnumNS):
		base = 0x310
		frames = (
			X('heartbeat', Status, 'dcStr', 'reserved') + base,
			X('error', Error),
			X('buttons', sw_Buttons),
			X('lights', sw_Lights))
class dc(Namespace):
	class rx(EnumNS):
		base = 0x110
		frames = (
			X('horn', UInt64, 'enabled') + base,
			X('signals', UInt2, 'l', 'r'),
			X('cruise', UInt64, 'enabled'),
			X('cruise_velocity_current', Float2, 'velocity', 'current'))
	class tx(EnumNS):
		base = ws20.rx.base
		frames = ()

class CAN:
	subspaces = (bms(), ws20(), mppt(), sw(), dc())
	Packet = """
			struct Packet {
				union frame_t {
					uint64_t data;
					uint8_t bytes[8];
				} frame;
				static const uint32_t _id = 0;
				virtual uint32_t id() const {return _id;}
				virtual uint64_t data() const {return frame.data;}
				virtual uint64_t &data() {return frame.data;}
				virtual uint8_t  *bytes() {return frame.bytes;}
				virtual const uint8_t *bytes() const {return frame.bytes;}
				virtual ~Packet() {}
				ALWAYSINLINE Packet(): frame{0} {}
				ALWAYSINLINE Packet(const uint64_t _i): frame{_i} {}
				ALWAYSINLINE Packet(const Packet& p): frame{p.data()} {}
				Packet& operator= (const uint64_t _i) {data() = _i; return *this;}
				Packet& operator= (const Packet& p) {data() = p.data(); return *this;}
			};\n"""
	def __str__(self):
		contents = ''.join(str(x) for x in self.subspaces)
		contents = namespace % ('frame', self.Packet + contents)
		return namespace % ('can', contents)

class can_def:
	docstring = """
	/**
	 * CAN frames are implemented as PACKED structs,
	 * within namespaces for scoping.
	 *
	 * CAN addresses are uint16_t's,
	 * within enum classes for scoping.
	 */\n\n"""
	def __str__(self):
		return self.docstring + namespace % ('nu', str(CAN()))

def main():
	f = open('can_def.hpp', 'w')
	contents = can_def()
	f.write(str(contents))

if __name__ == '__main__':
	main()






