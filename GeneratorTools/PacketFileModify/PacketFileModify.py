
import argparse
import os
import jinja2
import FileModifySource
import FileModifyHeader

def main():
    arg_parser = argparse.ArgumentParser(description = 'PacketFileModify')
    arg_parser.add_argument('--name', type=str, default='', help='file name')
    arg_parser.add_argument('--target', type=str, default='', help='file target')
    args = arg_parser.parse_args()

    if args.target == 'header':
        PacketFile = FileModifyHeader.FileModifyHeader()
        PacketFile.file_modify(args.name)

    elif args.target == 'source':
        PacketFile = FileModifySource.FileModifySource()
        PacketFile.file_modify(args.name)

    return

if __name__ == "__main__":
    main()