import Options
from os import unlink, symlink, popen
from os.path import exists 

srcdir = "./src"
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "node-ewah"
  obj.source = "EWAHAddon.cc"

def shutdown():
  if Options.commands['clean']:
    if exists('node-ewah.node'): unlink('node-ewah.node')
  else:
    if exists('build/default/node-ewah.node') and not exists('node-ewah.node'):
      symlink('build/default/node-ewah.node', 'node-ewah.node')

