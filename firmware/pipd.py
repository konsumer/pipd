import sys
from luma.core import cmdline, error
from luma.core.sprite_system import framerate_regulator
from PIL import Image, ImageDraw
import socket
from threading import Thread

def display_settings(device, args):
  iface = ''
  display_types = cmdline.get_display_types()
  if args.display not in display_types['emulator']:
    iface = f'Interface: {args.interface}\n'

  lib_name = cmdline.get_library_for_display_type(args.display)
  if lib_name is not None:
    lib_version = cmdline.get_library_version(lib_name)
  else:
    lib_name = lib_version = 'unknown'

  import luma.core
  version = f'luma.{lib_name} {lib_version} (luma.core {luma.core.__version__})'

  return f'Version: {version}\nDisplay: {args.display}\n{iface}Dimensions: {device.width} x {device.height}\n{"-" * 60}'


def get_device(actual_args=None):
  if actual_args is None:
    actual_args = sys.argv[1:]
  parser = cmdline.create_parser(description='luma.examples arguments')
  args = parser.parse_args(actual_args)

  if args.config:
    # load config from file
    config = cmdline.load_config(args.config)
    args = parser.parse_args(config + actual_args)

  # create device
  try:
    device = cmdline.create_device(args)
    print(display_settings(device, args))
    return device

  except error.Error as e:
    parser.error(e)
    return None
