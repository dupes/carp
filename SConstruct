import glob

#get the mode flag from the command line
#default to 'release' if the user didn't specify
mymode = ARGUMENTS.get('mode', 'debug')   #holds current mode

#check if the user has been naughty: only 'debug' or 'release' allowed
if not (mymode in ['debug', 'release']):
   print "Error: expected 'debug' or 'release', found: " + mymode
   Exit(1)

#tell the user what we're doing
print '**** Compiling in ' + mymode + ' mode...'

debugcflags = ['-Wall', '-O0', '-g3']   #extra compile flags for debug
releasecflags = ['-O2', '-DNDEBUG']         #extra compile flags for release
libs = ['X11', 'opencv_highgui', 'opencv_imgproc', 'opencv_contrib', 'opencv_legacy', 'opencv_core', 
        'opencv_video', 'sqlite3', 'opencv_ml']

env = Environment()

shared = glob.glob('shared/*.cpp')

for x in range(len(shared)):
    shared[x] = "../" + shared[x]

#make sure the sconscripts can get to the variables
Export('env', 'mymode', 'debugcflags', 'releasecflags', 'shared', 'libs')

#put all .sconsign files in one place
env.SConsignFile()

#specify the sconscript for myprogram
project = 'extract'
SConscript(project + '/sconscript', exports=['project'])

project = 'record'
SConscript(project + '/sconscript', exports=['project'])

project = 'test1'
SConscript(project + '/sconscript', exports=['project'])

project = 'label'
SConscript(project + '/sconscript', exports=['project'])

project = 'view_object'
SConscript(project + '/sconscript', exports=['project'])

project = 'classify'
SConscript(project + '/sconscript', exports=['project'])

project = 'test_classifier'
SConscript(project + '/sconscript', exports=['project'])
