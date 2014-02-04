#!/usr/bin/env python3

import argparse
from textwrap import dedent, indent


### The possible memory layouts of a CAN frame: ###

class Layout:
	""" Static Class.
	Contains bit layouts for CAN packets. When we define the CAN
	namespace below, each CAN address declares its bit layout with one of these.
	"""
	Empty = []

	Module = [
		'UInt32',
		'Single'
	]

	Float64x1 = [
		'Double'
	]

	Float32x2 = [
		'Single',
		'Single'
	]

	UInt64x1 = [
		'UInt64'
	]

	UInt32x2 = [
		'UInt32',
		'UInt32'
	]

	UInt16x4 = [
		'UInt16',
		'UInt16',
		'UInt16',
		'UInt16'
	]

	Int16x4 = [
		'Int16',
		'Int16',
		'Int16',
		'Int16'
	]

	Trip = [
		'Int16',
		'UInt16',
		'UInt16',
		'UInt16'
	]

	Status = [
		'UInt32',  # ACTUALLY supposed to be char[4] in c++
		'UInt32'
	]


### Internal representations of Abstract Syntax Elements for generated sourcecode: ###

class CanBasePacket:
	cpp_class_packet = """\
	struct Packet
	{
		union frame_t
		{
			uint64_t data;
			uint8_t bytes[8];
		};

	private:
		static const uint32_t _id = 0;
		frame_t _frame;

	public:
		virtual uint32_t id() const {return _id;}
		frame_t &frame() {return _frame;}
		frame_t frame() const {return _frame;}

		uint64_t &data() {return frame().data;}
		uint64_t data() const {return frame().data;}

		uint8_t *bytes() {return frame().bytes;}
		const uint8_t *bytes() const {return frame().bytes;}

		virtual ~Packet() {}
		Packet(): _frame{0} {}
		Packet(const uint64_t _data): _frame{_data} {}
		Packet(const Packet& p): _frame{p.data()} {}
		Packet& operator= (const uint64_t _data) {data() = _data; return *this;}
		Packet& operator= (const Packet& p) {data() = p.data(); return *this;}
	};"""
	cpp_class_packet = dedent(cpp_class_packet)

	cs_class_packet = """\
	class Packet
	{
		[StructLayout(LayoutKind.Explicit)]
		public struct UInt64x1
		{
			[FieldOffset(0)]
			public UInt64 uint64_0;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct Float64x1
		{
			[FieldOffset(0)]
			public Double double_0;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct Float32x2
		{
			[FieldOffset(0)]
			public Single single_0;
			[FieldOffset(4)]
			public Single single_1;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct UInt32x2
		{
			[FieldOffset(0)]
			public UInt32 uint32_0;
			[FieldOffset(4)]
			public UInt32 uint32_1;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct UInt16x4
		{
			[FieldOffset(0)]
			public UInt16 uint16_0;
			[FieldOffset(2)]
			public UInt16 uint16_1;
			[FieldOffset(4)]
			public UInt16 uint16_2;
			[FieldOffset(6)]
			public UInt16 uint16_3;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct Int16x4
		{
			[FieldOffset(0)]
			public Int16 int16_0;
			[FieldOffset(2)]
			public Int16 int16_1;
			[FieldOffset(4)]
			public Int16 int16_2;
			[FieldOffset(6)]
			public Int16 int16_3;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct Status
		{
			[FieldOffset(0)]
			public UInt32 uint32_0;
			[FieldOffset(4)]
			public UInt32 uint32_1;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct Trip
		{
			[FieldOffset(0)]
			public Int16 int16_0;
			[FieldOffset(2)]
			public UInt16 uint16_1;
			[FieldOffset(4)]
			public UInt16 uint16_2;
			[FieldOffset(6)]
			public UInt16 uint16_3;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct UInt8x8
		{
			[FieldOffset(0)]
			public Byte byte_0;
			[FieldOffset(1)]
			public Byte byte_1;
			[FieldOffset(2)]
			public Byte byte_2;
			[FieldOffset(3)]
			public Byte byte_3;
			[FieldOffset(4)]
			public Byte byte_4;
			[FieldOffset(5)]
			public Byte byte_5;
			[FieldOffset(6)]
			public Byte byte_6;
			[FieldOffset(7)]
			public Byte byte_7;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct Frame
		{
			[FieldOffset(0)]
			public UInt64 data;
			[FieldOffset(0)]
			public Float64x1 float64x1;
			[FieldOffset(0)]
			public Float32x2 float32x2;
			[FieldOffset(0)]
			public UInt64x1 uint64x1;
			[FieldOffset(0)]
			public UInt32x2 uint32x2;
			[FieldOffset(0)]
			public UInt16x4 uint16x4;
			[FieldOffset(0)]
			public Int16x4 int16x4;
			[FieldOffset(0)]
			public UInt8x8 uint8x8;
			[FieldOffset(0)]
			public Trip trip;
			[FieldOffset(0)]
			public Status status;
		}

		UInt16 id = 0;
		Byte length = 0;
		public Frame frame = new Frame();

		public UInt16 ID { get { return this.id; } }

		public Byte Length { get { return this.length; } }

		public UInt64 Data { get { return this.frame.data; } }

		/// <summary>
		/// Initializes a new instance of the <see cref="SolarCar.Can.Packet"/> class.
		/// </summary>
		/// <param name="id">CAN ID, maximum 0x7FF for standard CAN.</param>
		/// <param name="length">Length of CAN Data.</param>
		/// <param name="data">Data in CAN frame.</param>
		public Packet(UInt16 id, Byte length, UInt64 data)
		{
			this.id = id;
			this.length = length;
			this.frame.data = data;
		}
	}"""
	cs_class_packet = dedent(cs_class_packet)

	def get_cpp(self):
		return self.cpp_class_packet

	def get_cs(self):
		return self.cs_class_packet


class CanPacket:
	cpp_indented_field_template = '\t\t\t%(field_type)s %(name)s;'

	cpp_class_template = """\
	class %(name)s: public Packet
	{
		static const uint32_t _id = %(id)s;
	public:
		union layout_t
		{
			uint64_t data;
			uint8_t bytes[8];
			struct PACKED
			{
	%(indented_fields)s
			};
		};
		uint32_t id() const {return _id;}
		layout_t &frame() {return *(layout_t *)&Packet::frame();}
		layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
		%(name)s(): Packet(0) {}
		%(name)s(const uint64_t _i): Packet(_i) {}
		%(name)s(const Packet& p): Packet(p.data()) {}
	};"""
	cpp_class_template = dedent(cpp_class_template)

	cs_property_template = """\
	public %(field_type)s %(field_name)s
	{
		get { return this.frame.%(lower_frame_type)s.%(lower_field_type)s_%(index)i; }
		set { this.frame.%(lower_frame_type)s.%(lower_field_type)s_%(index)i = value; }
	}
	"""
	cs_property_template = dedent(cs_property_template)

	cs_class_template = """\
	class %(name)s: Packet
	{
		public %(name)s(UInt64 data = 0) : base(%(id)s, 8, data)
		{
		}

		public const UInt16 _id = %(id)s;

		%(indented_fields)s
	}"""
	cs_class_template = dedent(cs_class_template)

	def __init__(self, name='error', layout=Layout.Empty, field_names=(), id=0):
		if type(name) is not str:
			raise TypeError("type of [name] for CanPacket must be <str>.")
		if type(layout) not in (list, tuple):
			raise TypeError("type of [layout] for CanPacket must be iterable.")
		if type(field_names) not in (list, tuple):
			raise TypeError("type of [field_names] for CanPacket must be iterable.")
		if type(id) is not int:
			raise TypeError("type of [id] for CanPacket must be <int>.")

		self.name = name
		self.id = hex(id)
		self.layout = layout
		self.field_names = field_names

		# EVIL BLACK MAGIC to get c# type name
		self.lower_frame_type = next(k for k, v in Layout.__dict__.items() if v == layout).lower()

	def get_cpp(self):
		'''Returns C++ source code for this CanPacket.'''

		# construct struct members, indent, & join with newlines
		members = []
		for field_type, name in zip(self.layout, self.field_names):
			try:
				format_args = {'field_type': field_type, 'name': name}
				members.append(self.cpp_indented_field_template % format_args)
			except TypeError as e:
				print('name = ' + self.name)
				print('field_type = ' + field_type)
				print("member = " + name)
				raise e
		members = '\n'.join(members)

		format_args = {'name': self.name, 'id': self.id, 'indented_fields': members}
		return self.cpp_class_template % format_args

	def get_cs(self):
		''' Returns C# source code for this CanPacket.'''

		# construct struct members, indent, & join with newlines
		members = []
		for field_type, name, index in zip(self.layout, self.field_names, range(len(self.layout))):
			try:
				format_args = {'field_name': name, 'index': index,
							   'field_type': field_type, 'lower_field_type': field_type.lower(),
							   'lower_frame_type': self.lower_frame_type}
				members.append(indent(self.cs_property_template % format_args, '\t'))
			except TypeError as e:
				print('name = ' + self.name)
				print('field_type = ' + field_type)
				print("member = " + name)
				raise e
		members = '\n'.join(members)

		format_args = {'name': self.name, 'id': self.id, 'indented_fields': members}
		return self.cs_class_template % format_args


class CanEnum:
	cpp_enum_template = """\
	enum %(name)s
	{
	%(indented_fields)s
	};"""
	cpp_enum_template = dedent(cpp_enum_template)

	def __init__(self, name):
		if type(name) is not str:
			raise TypeError("Type of [name] must be <str>.")

		self.name = name
		self.constants = []

	def add_constant(self, name, number):
		if type(name) is not str:
			raise TypeError("Type of [name] must be <str>.")
		if type(number) is not str:
			if type(number) is int:
				number = hex(number)
			else:
				raise TypeError("Type of [number] must be <int>.")

		self.constants.append([name, number])

	def get_cpp(self):
		indented_fields = ('\t%s = %s' % (name, number) for name, number in self.constants)
		indented_fields = ',\n'.join(indented_fields)
		format_args = {'name': self.name, 'indented_fields': indented_fields}
		return self.cpp_enum_template % format_args

	def get_cs(self):
		return ""


class Namespace:
	cpp_namespace_template = """\
	namespace %(name)s
	{
	%(indented_contents)s
	}"""
	cpp_namespace_template = dedent(cpp_namespace_template)

	cs_namespace_template = """\
	namespace %(name)s
	{
	%(indented_contents)s
	}"""
	cs_namespace_template = dedent(cs_namespace_template)

	def __init__(self, name='default'):
		if type(name) is not str:
			raise TypeError('Type of [name] must be <str>.')

		self.name = name
		self.contents = []

	def add_namespace(self, the_namespace):
		if type(the_namespace) is not Namespace:
			raise TypeError('Type of argument must be <Namespace>.')

		self.contents.append(the_namespace)

	def add_class(self, the_class):
		'''Appends a CanPacket or CanBasePacket class.'''
		if type(the_class) not in (CanPacket, CanBasePacket):
			raise TypeError('Type of argument must be <CanPacket>.')

		self.contents.append(the_class)

	def add_enum(self, the_enum):
		'''Appends a CanEnum.'''
		if type(the_enum) is not CanEnum:
			raise TypeError('Type of the_enum must be <CanEnum>.')

		self.contents.append(the_enum)

	def get_cpp(self):
		contents = (indent(the_class.get_cpp(), '\t') for the_class in self.contents)
		contents = '\n\n'.join(contents)
		format_args = {'name': self.name, 'indented_contents': contents}
		return self.cpp_namespace_template % format_args

	def get_cs(self):
		contents = (indent(the_class.get_cs(), '\t') for the_class in self.contents
			if not isinstance(the_class, CanEnum))
		contents = '\n\n'.join(contents)
		format_args = {'name': self.name, 'indented_contents': contents}
		return self.cs_namespace_template % format_args

### Serialized CAN definitions: ###

can_def = [
	{
		'type': 'namespace',
		'name': 'bps',
		'contents': [
			{
				'type': 'enumerated_namespace',
				'name': 'rx',
				'base': 0x200,
				'contents': [
					['trip',			Layout.Trip,	['trip_code', 'module']],
					['reset_cc_batt',	Layout.Empty],
					['reset_cc_array',	Layout.Empty],
					['reset_cc_mppt1',	Layout.Empty],
					['reset_cc_mppt2',	Layout.Empty],
					['reset_cc_mppt3',	Layout.Empty],
					['reset_cc_Wh',		Layout.Empty],
					['reset_cc_all',	Layout.Empty]
					# MAX = 0x20F
				]
			},
			{
				'type': 'enumerated_namespace',
				'name': 'tx',
				'base': 0x210,
				'contents': [
					['heartbeat',	Layout.Status,	['bms_str', 'uptime_s']],
					['errors',		Layout.Int16x4,		['error', 'error_value',
														 'last_error', 'last_error_value']],
					['bps_status',	Layout.UInt16x4,	['mode', 'disabled_module',
														 'reserved', 'reserved1']],
					['current',		Layout.UInt16x4,	['array', 'battery',
														 'reserved', 'reserved1']],
					['voltage_temp', Layout.UInt16x4,	['module', 'voltage',
														 'temp', 'reserved']],
					['cc_array',	Layout.Float64x1,	['count', ]],
					['cc_batt',		Layout.Float64x1,	['count', ]],
					['cc_mppt1',	Layout.Float64x1,	['count', ]],
					['cc_mppt2',	Layout.Float64x1,	['count', ]],
					['cc_mppt3',	Layout.Float64x1,	['count', ]],
					['Wh_batt',		Layout.Float64x1,	['count', ]],
					['Wh_mppt1',	Layout.Float64x1,	['count', ]],
					['Wh_mppt2',	Layout.Float64x1,	['count', ]],
					['Wh_mppt3',	Layout.Float64x1,	['count', ]],
					['trip_pt',		Layout.Trip,	['trip_code', 'module',
													 'low_current', 'high_current']],
					['trip_pt_voltage_temp',	Layout.UInt16x4,
						['low_volt', 'high_volt', 'low_temp', 'high_temp']],
				]
			}
		]
	},
	{
		'type': 'namespace',
		'name': 'ws20',
		'contents': [
			{
				'type': 'enumerated_namespace',
				'name': 'rx',
				'base': 0x500,  # == Tritium's Driver Controls' tx base but we don't use that.
				'contents': [
					['driver_controls_id', Layout.Status, ['drvId', 'serialNo']],
					['drive_cmd', Layout.Float32x2, ['motorVelocity', 'motorCurrent']],
					['power_cmd', Layout.Float32x2, ['reserved', 'busCurrent']],
					['reset_cmd', Layout.UInt32x2, ['unused1', 'unused2']]
					# MAX = 0x510 ?
				]
			},
			{
				'type': 'enumerated_namespace',
				'name': 'tx',
				'base': 0x400,
				'contents': [
					['motor_id',		Layout.Status,		['tritiumId', 'serialNo']],
					['motor_status_info', Layout.UInt16x4,	['limitFlags', 'errorFlags',
															 'activeMotor', 'reserved']],
					['motor_bus',		Layout.Float32x2,	['busVoltage', 'busCurrent']],
					['motor_velocity',	Layout.Float32x2,	['motorVelocity', 'vehicleVelocity']],
					['motor_phase',		Layout.Float32x2,	['phaseBCurrent', 'phaseACurrent']],
					['voltage_vector',	Layout.Float32x2,	['voltageIm', 'voltageRe']],
					['current_vector',	Layout.Float32x2,	['currentIm', 'currentRe']],
					['backemf',			Layout.Float32x2,	['backEmfIm', 'backEmfRe']],
					['rail_15v_1pt65v',	Layout.Float32x2,	['onePtSixtyFiveVRef', 'fifteenVPowerRail']],
					['rail_2pt5v_1pt2v', Layout.Float32x2,	['onePtTwoVSupply', 'twoPtFiveVSupply']],
					['fanspeed',		Layout.Float32x2,	['fanDrive', 'fanRpm']],
					['sinks_temp',		Layout.Float32x2,	['motorTemp', 'heatsinkTemp']],
					['cpu_airin_temp',	Layout.Float32x2,	['processorTemp', 'airInletTemp']],
					['cap_airout_temp',	Layout.Float32x2,	['capacitorTemp', 'airOutTemp']],
					['odom_bus_ah',		Layout.Float32x2,	['odom', 'dcBusAmpHours']]
					# MAX = 0x4FF
				]
			}
		]
	},
	{
		'type': 'namespace',
		'name': 'pedals',
		'contents': [
			{
				'type': 'enumerated_namespace',
				'name': 'rx',
				'base': 0x100,
				'contents': [
					['cruise', Layout.Float32x2, ['velocity', 'current']]
				]
			},
			{
				'type': 'enumerated_namespace',
				'name': 'tx',
				'base': 0x110,
				'contents': [
					['pedals', Layout.UInt16x4, ['accel_pedal', 'regen_pedal', 'brake_pedal', 'reserved1']]
				]
			},
		]
	},
	{
		'type': 'namespace',
		'name': 'os',
		'contents': [
			{
				'type': 'enumerated_namespace',
				'name': 'tx',
				'base': 0x310,
				'contents': [
					['user_cmds', Layout.UInt16x4, ['power', 'gearFlags', 'signalFlags', 'reserved']]
				]
			},
		]
	},
	{
		'type': 'namespace',
		'name': 'mppt',
		'contents': [
			{
				'type': 'enumerated_namespace',
				'name': 'rx',
				'base': 0b11100010000,  # 0x710
				'contents': [
					['mppt', Layout.Empty]
				]
			},
			{
				'type': 'enumerated_namespace',
				'name': 'tx',
				'base': 0b11100010000,  # 0x710
				'contents': [
					['mppt', Layout.Empty]
				]
			}
		]
	},
]


### A source-code generating deserializer: ###

class Deserializer:
	'''Parses the serialized CAN definitions and creates internal representations
	using Namespace and CanPacket objects. Produces source code.
	'''

	cpp_surrounding_file = """\
	/*
	 * CAN frames are implemented as PACKED structs,
	 * within namespaces for scoping.
	 *
	 * CAN addresses are uint16_t's,
	 * within enum classes for scoping.
	 */
	#include "nu/compiler.h"
	#include <cstdint>
	#pragma GCC diagnostic ignored "-pedantic"
	#define UInt64 uint64_t
	#define UInt32 uint32_t
	#define UInt16 uint16_t
	#define Int16 int16_t
	#define Byte uint8_t
	#define Char int8_t
	#define Double double
	#define Single float
	\n
	%(contents)s
	\n
	#undef UInt64
	#undef UInt32
	#undef UInt16
	#undef Int16
	#undef Byte
	#undef Char
	#undef Double
	#undef Single
	#pragma GCC diagnostic warning "-pedantic"
	\n"""
	cpp_surrounding_file = dedent(cpp_surrounding_file)

	cs_surrounding_file = """\
	using System;
	using System.Runtime.InteropServices;
	\n
	%(contents)s
	\n"""
	cs_surrounding_file = dedent(cs_surrounding_file)

	def __init__(self, data=can_def):
		addr_namespace = Namespace('Addr')
		for outer_namespace_data in data:
			outer_namespace = Namespace(outer_namespace_data['name'])
			for subspace_data in outer_namespace_data['contents']:
				outer_namespace.add_namespace(self.deserialize_namespace(subspace_data))
			addr_namespace.add_namespace(outer_namespace)

		can_namespace = Namespace('Can')
		can_namespace.add_class(CanBasePacket())
		can_namespace.add_namespace(addr_namespace)
		self.nu = Namespace('nu')
		self.nu.add_namespace(can_namespace)

	def deserialize_namespace(self, namespace_data):
		name = namespace_data['name']
		the_namespace = Namespace(name)

		base = namespace_data['base']
		contents = namespace_data['contents']
		ids = list(range(base, base+len(contents)))

		# create classes
		classes = []
		for class_data, id in zip(contents, ids):
			# unpack class_data into CanPacket arguments
			classes.append(CanPacket(*class_data, id=id))

		# create enum
		enum = CanEnum('addresses')
		for the_class in classes:
			enum.add_constant(the_class.name + '_k', the_class.id)

		# add classes and enum to the_namespace
		the_namespace.add_enum(enum)
		for the_class in classes:
			the_namespace.add_class(the_class)

		return the_namespace

	def get_cpp(self):
		return self.cpp_surrounding_file % {'contents': self.nu.get_cpp()}

	def get_cs(self):
		self.nu.name = "SolarCar"
		return self.cs_surrounding_file % {'contents': self.nu.get_cs()}

	def get_root_namespace(self):
		return self.nu


def main():
	parser = argparse.ArgumentParser(description='Generate a CAN namespace.')
	group = parser.add_mutually_exclusive_group(required=True)
	group.add_argument('--cpp', dest='lang', action='store_const', const='cpp',
		help='Specify C++ as output language.')
	group.add_argument('--cs', dest='lang', action='store_const', const='cs',
		help='Specify C# as output language.')

	parser.add_argument('--file', default=None,
		help='Save generated source code to the provided filename.')
	parser.add_argument('--stdout', action='store_true', default=False,
		help='Print generated source code to stdout.')

	args = parser.parse_args()
	data = Deserializer()

	if args.lang == 'cpp':
		source = data.get_cpp()
	elif args.lang == 'cs':
		source = data.get_cs()
	else:
		raise RuntimeError("Error: Must specify an output language.")

	if args.file is not None:
		with open(args.file, 'w+') as file:
			file.write(source + '\n')

	if args.stdout:
		print(source)

if __name__ == '__main__':
	main()
