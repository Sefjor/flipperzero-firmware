#!/usr/bin/env python3

import logging
import argparse
import subprocess
import sys
import os

from flipper.app import App
from flipper.cube import CubeProgrammer


class Main(App):
    def init(self):
        self.subparsers = self.parser.add_subparsers(help="sub-command help")
        self.parser_check = self.subparsers.add_parser(
            "check", help="Check Option Bytes"
        )
        self.parser_check.add_argument(
            "--port", type=str, help="Port to connect: swd or usb1", default="swd"
        )
        self.parser_check.set_defaults(func=self.check)
        # Set command
        self.parser_set = self.subparsers.add_parser("set", help="Set Option Bytes")
        self.parser_set.add_argument(
            "--port", type=str, help="Port to connect: swd or usb1", default="swd"
        )
        self.parser_set.set_defaults(func=self.set)
        # OB
        self.ob = {}

    def before(self):
        self.logger.info(f"Loading Option Bytes data")
        file_path = os.path.join(os.path.dirname(sys.argv[0]), "ob.data")
        file = open(file_path, "r")
        for line in file.readlines():
            k, v, o = line.split(":")
            self.ob[k.strip()] = v.strip(), o.strip()

    def check(self):
        self.logger.info(f"Checking Option Bytes")
        cp = CubeProgrammer(self.args.port)
        if cp.checkOptionBytes(self.ob):
            self.logger.info(f"OB Check OK")
            return 0
        else:
            self.logger.error(f"OB Check FAIL")
            return 255

    def set(self):
        self.logger.info(f"Setting Option Bytes")
        cp = CubeProgrammer(self.args.port)
        if cp.setOptionBytes(self.ob):
            self.logger.info(f"OB Set OK")
            return 0
        else:
            self.logger.error(f"OB Set FAIL")
            return 255


if __name__ == "__main__":
    Main()()
