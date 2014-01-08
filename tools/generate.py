#!/usr/bin/env python3

import argparse
from textwrap import dedent, indent

class Layout:
	""" Static Class.
	Contains bit layouts for CAN packets. When we define the CAN
	namespace below, each CAN address declares its bit layout with one of these.
	"""
	Empty = []

	Module = [
		'uint32_t %s',
		'float %s'
	]

	Double = [
		'double %s'
	]

	Float2 = [
		'float %s',
		'float %s'
	]

	UInt64 = [
		'uint64_t %s'
	]

	UInt2 = [
		'uint32_t %s',
		'uint32_t %s'
	]

	UInt16x4 = [
		'uint16_t %s',
		'uint16_t %s',
		'uint16_t %s',
		'uint16_t %s'
	]

	Trip = [
		'int32_t %s',
		'uint32_t %s'
	]

	Error = [
		'char %s[8]'
	]

	Status = [
		'char %s[4]',
		'uint32_t %s'
	]

	PedalBoard = [
		'uint16_t %s', # accel pedal
		'uint16_t %s', # regen pedal
		'uint32_t %s' # bool brake
	]

	CustomBitField = [
		'%s'
	]


class BitFields:
	OSStatus = [
		['power', 1],
		['drive', 1],
		['reverse', 1],
		['signals', 2],
		['headlights', 1],
		['horn', 1]
	]


class CanPacket:
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

	def __init__(self, name = 'error', layout = Layout.Empty, field_names = (), id = 0):
		if type(name) is not str:
			raise TypeError("type of [name] for CanPacket must be <str>.")
		if type(layout) not in (list, tuple):
			raise TypeError("type of [layout] for CanPacket must be iterable.")
		if type(field_names) not in (list, tuple):
			raise TypeError("type of [field_names] for CanPacket must be iterable.")
		if type(id) is not int:
			raise TypeError("type of [id] for CanPacket must be <int>.")

		self.name = name
		self.layout = layout
		self.field_names = field_names
		self.id = hex(id)

	def get_cpp(self):
		'''Returns C++ source code for this CanPacket.'''
		if self.layout is Layout.CustomBitField:
			raise NotImplementedError("TODO")

		# construct struct members, indent, & join with newlines
		members = []
		for layout, name in zip(self.layout, self.field_names):
			try:
				members.append('\t\t\t' + layout % name + ';')
			except TypeError as e:
				print('name = ' + self.name)
				print('layout = ' + layout)
				print("members = " + name)
				raise e
		members = '\n'.join(members)

		format_args = {'name': self.name, 'id': self.id, 'indented_fields': members}
		return self.cpp_class_template % format_args


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

	def get_cpp(self):
		return self.cpp_class_packet

	def get_cs(self):
		raise NotImplementedError("TODO")


class CanEnum:
	cpp_enum_template = """\
	enum %(name)s
	{
	%(indented_fields)s
	};
	"""
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
		indented_fields = ['\t%s = %s' % (name, number) for name, number in self.constants]
		indented_fields = ',\n'.join(indented_fields)

		format_args = {'name': self.name, 'indented_fields': indented_fields}
		return self.cpp_enum_template % format_args


class Namespace:
	cpp_namespace_template = """\
	namespace %(name)s
	{
	%(indented_contents)s
	}"""
	cpp_namespace_template = dedent(cpp_namespace_template)

	def __init__(self, name = 'default'):
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
		contents = []
		for the_class in self.contents:
			contents.append(indent(the_class.get_cpp(), '\t'))
		contents = '\n\n'.join(contents)

		format_args = {'name': self.name, 'indented_contents':contents}
		return self.cpp_namespace_template % format_args


	def get_cs(self):
		raise NotImplementedError("TODO")


### Serialized CAN definitions: ###

can_def = [
	{
		'name': 'bms',
		'contents': [
			{
				'name': 'rx',
				'base': 0x200,
				'contents': [
					['trip', Layout.Trip, ['trip_code', 'module']],
					['reset_cc_batt', Layout.Empty],
					['reset_cc_array', Layout.Empty],
					['reset_cc_mppt1', Layout.Empty],
					['reset_cc_mppt2', Layout.Empty],
					['reset_cc_mppt3', Layout.Empty],
					['reset_cc_Wh', Layout.Empty],
					['reset_cc_all', Layout.Empty]
					# MAX = 0x20F
				]
			},
			{
				'name': 'tx',
				'base': 0x210,
				'contents': [
					['heartbeat', Layout.Status, ('bms_str', 'reserved')],
					['error', Layout.Error, ['message']],
					['uptime', Layout.Double, ('seconds',)],
					['last_reset', Layout.Trip, ('last_reset_code', 'reserved')],
					['batt_bypass', Layout.Module, ('module', 'reserved')],
					['current', Layout.Float2, ('array', 'battery')],
					['cc_array', Layout.Double, ('count',)],
					['cc_batt', Layout.Double, ('count',)],
					['cc_mppt1', Layout.Double, ('count',)],
					['cc_mppt2', Layout.Double, ('count',)],
					['cc_mppt3', Layout.Double, ('count',)],
					['Wh_batt', Layout.Double, ('count',)],
					['Wh_mppt1', Layout.Double, ('count',)],
					['Wh_mppt2', Layout.Double, ('count',)],
					['Wh_mppt3', Layout.Double, ('count',)],
					['voltage', Layout.Module, ('module', 'voltage')],
					['owVoltage', Layout.Module, ('module', 'ow_voltage')],
					['temp', Layout.Module, ('sensor', 'temp')],
					['trip', Layout.Trip, ('trip_code', 'module')],
					['last_trip', Layout.Trip, ('trip_code', 'module')],
					['trip_pt_current', Layout.Float2, ('low', 'high')],
					['trip_pt_voltage', Layout.Float2, ('low', 'high')],
					['trip_pt_temp', Layout.Float2, ('low', 'high')]
				]
			}
		]
	},
	{
		'name': 'ws20',
		'contents': [
			{
				'name': 'rx',
				'base': 0x500, # == Tritium's Driver Controls' tx base but we don't use that.
				'contents': [
					['driver_controls_id', Layout.Status, ['drvId', 'serialNo']],
					['drive_cmd', Layout.Float2, ['motorVelocity', 'motorCurrent']],
					['power_cmd', Layout.Float2, ['reserved', 'busCurrent']],
					['reset_cmd', Layout.UInt2, ['unused1', 'unused2']]
					# MAX = 0x510 ?
				]
			},
			{
				'name': 'tx',
				'base': 0x400,
				'contents': [
					['motor_id', Layout.Status, ['tritiumId', 'serialNo']],
					['motor_status_info', Layout.UInt16x4, ['limitFlags', 'errorFlags', 'activeMotor', 'reserved']],
					['motor_bus', Layout.Float2, ['busVoltage', 'busCurrent']],
					['motor_velocity', Layout.Float2, ['motorVelocity', 'vehicleVelocity']],
					['motor_phase', Layout.Float2, ['phaseBCurrent', 'phaseACurrent']],
					['voltage_vector', Layout.Float2, ['voltageIm', 'voltageRe']],
					['current_vector', Layout.Float2, ['currentIm', 'currentRe']],
					['backemf', Layout.Float2, ['backEmfIm', 'backEmfRe']],
					['rail_15v_1pt65v', Layout.Float2, ['onePtSixtyFiveVRef', 'fifteenVPowerRail']],
					['rail_2pt5v_1pt2v', Layout.Float2, ['onePtTwoVSupply', 'twoPtFiveVSupply']],
					['fanspeed', Layout.Float2, ['fanDrive', 'fanRpm']],
					['sinks_temp', Layout.Float2, ['motorTemp', 'heatsinkTemp']],
					['cpu_airin_temp', Layout.Float2, ['processorTemp', 'airInletTemp']],
					['cap_airout_temp', Layout.Float2, ['capacitorTemp', 'airOutTemp']],
					['odom_bus_ah', Layout.Float2, ['odom', 'dcBusAmpHours']]
					# MAX = 0x4FF
				]
			}
		]
	},
	{
		'name': 'pedals',
		'contents': [
			{
				'name': 'rx',
				'base': 0x100,
				'contents': [
					['cruise', Layout.Float2, ['velocity', 'current']]
				]
			},
			{
				'name': 'tx',
				'base': 0x110,
				'contents': [
					['pedals', Layout.UInt16x4, ['accel_pedal', 'brake_pedal', 'reserved', 'reserved1']]
				]
			},
		]
	},
	{
		'name': 'os',
		'contents': [
			{
				'name': 'tx',
				'base': 0x310,
				'contents': [
					['driver_input', Layout.UInt16x4, ['power', 'gearFlags', 'lightsFlags', 'horn']]
				]
			},
		]
	},
	{
		'name': 'mppt',
		'contents': [
			{
				'name': 'rx',
				'base': 0b11100010000, # 0x710
				'contents': [
					['mppt', Layout.Empty]
				]
			},
			{
				'name': 'tx',
				'base': 0b11100010000, # 0x710
				'contents': [
					['mppt', Layout.Empty]
				]
			}
		]
	},
]


class Unarchiver:
	'''Parses the serialized CAN definitions and creates internal representations
	using Namespace and CanPacket objects. Produces source code.
	'''

	cpp_comment = """\
	/*
	 * CAN frames are implemented as PACKED structs,
	 * within namespaces for scoping.
	 *
	 * CAN addresses are uint16_t's,
	 * within enum classes for scoping.
	 */
	\n"""
	cpp_comment = dedent(cpp_comment)

	def __init__(self, data=can_def):
		frame_namespace = Namespace('frame')
		frame_namespace.add_class(CanBasePacket())

		for outer_namespace_data in data:
			outer_namespace = Namespace(outer_namespace_data['name'])
			for subspace_data in outer_namespace_data['contents']:
				outer_namespace.add_namespace(self.deserialize_namespace(subspace_data))
			frame_namespace.add_namespace(outer_namespace)

		can = Namespace('can')
		can.add_namespace(frame_namespace)
		self.nu = Namespace('nu')
		self.nu.add_namespace(can)


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
		return self.cpp_comment + self.nu.get_cpp()

	def get_cs(self):
		raise NotImplementedError("TODO")

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
	data = Unarchiver()

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

