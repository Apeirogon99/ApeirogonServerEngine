
import argparse
import jinja2
import ProtoParser

def main():

	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	arg_parser.add_argument('--path', type=str, default='', help='proto path')
	arg_parser.add_argument('--output', type=str, default='', help='output file')
	arg_parser.add_argument('--recv', type=str, default='', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='', help='send convention')
	arg_parser.add_argument('--target', type=str, default='', help='target')
	args = arg_parser.parse_args()

	parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	parser.parse_proto(args.path)

	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	if args.target == 'server':
		template = env.get_template('PacketHandler.h')
		output = template.render(parser=parser, output=args.output)
		f = open(args.output+'.h', 'w+')
		f.write(output)
		f.close()
	elif args.target == 'unreal':
		UEtemplate = env.get_template('UEPacketHandler.h');
		UEoutput = UEtemplate.render(parser=parser, output=args.output)
		UEf = open(args.output+'.h', 'w+')
		UEf.write(UEoutput)
		UEf.close()

	#print(output)
	return

if __name__ == '__main__':
	main()