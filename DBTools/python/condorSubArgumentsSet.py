#!/usr/bin/env python

import socket

hostname = socket.getfqdn()
lpcCAF = ('fnal.gov' in hostname)

CondorSubArgumentsSet = {}
if lpcCAF:
    CondorSubArgumentsSet = {
        1   :  {'Executable'             :  ''},
        2   :  {'Universe'               :  'vanilla'},
        3   :  {'request_memory '        : '2048MB'},
        4   :  {'Arguments'              :  ''},
        5   :  {'Output'                 :  'condor_$(Process).out'},
        6   :  {'Error'                  :  'condor_$(Process).err'},
        7   :  {'Log'                    :  'condor_$(Process).log\n'},
        8   :  {'Transfer_Input_files'   :  ''},
        9   :  {'Transfer_Output_files'  :  ''},
    }
else:
    CondorSubArgumentsSet = {
        1   :  {'Executable'             :  ''},
        2   :  {'Universe'               :  'vanilla'},
        3   :  {'Getenv'                 :  'True'},
        4   :  {'request_memory '        : '2048MB'},
        5   :  {'Arguments'              :  ''},
        6   :  {'Output'                 :  'condor_$(Process).out'},
        7   :  {'Error'                  :  'condor_$(Process).err'},
        8   :  {'Log'                    :  'condor_$(Process).log\n'},
        9   :  {'Transfer_Input_files'   :  ''},
        10  :  {'Transfer_Output_files'  :  ''},
    }
