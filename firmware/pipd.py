import sys
from luma.core import cmdline, error
from luma.core.sprite_system import framerate_regulator
from PIL import Image, ImageDraw
import socket
from threading import Thread

def is_open(ip,port):
 s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
 try:
  s.connect((ip, int(port)))
  s.shutdown(2)
  return True
 except:
  return False

def run_loop(callback):
  device = get_device()
  screen = Image.new("1", device.size)
  draw = ImageDraw.Draw(screen)
  g = True
  while g:
    g = callback(screen, device, draw)

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


# this will process messages from puredata
class PuredataServer(Thread):
  def __init__(self, host='localhost', port=13001):
    Thread.__init__(self)
    self.running = True
    self.device = get_device()
    self.screen = Image.new("1", self.device.size)
    self.draw = ImageDraw.Draw(self.screen)
    self.server =  socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    self.server.bind((host, port))
    # TODO: setup button & LED

  def stop(self):
    self.running = False

  def run(self):
    # TODO: check button to toggle puredata running, use LED to show status
    while self.running:
      m, address = self.server.recvfrom(1024)
      args = m.decode().replace(";", "").split()
      command = args.pop(0)
      print(command, args)
      if command == 'text':
        self.draw.text((int(args[1]), int(args[2])),  " ".join(args[3:]), fill=int(args[0]))
      if command == 'rectangle':
        self.draw.rectangle([(int(args[1]), int(args[2])), (int(args[3]), int(args[4]))], fill=int(args[0]))
      if command == 'image':
        pass # TODO: work out how to cache loaded images
      if command == 'rgb':
        pass # TODO: work out how to chnage RGB colors

      self.device.display(self.screen)
