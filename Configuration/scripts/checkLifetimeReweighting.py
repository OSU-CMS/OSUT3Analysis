# this script prints out a bunch of info about your reweighting in configurationOptions.py
# so you can check if you're doing it right

from OSUT3Analysis.Configuration.configurationOptions import rulesForLifetimeReweighting

for sample in rulesForLifetimeReweighting:
    #if not sample.startswith('AMSB_chargino_100GeV_'): continue
    #if not sample.endswith('_94X'): continue
    if not sample.startswith('stopToLB1000_'): continue

    print 'Rules for sample:', sample

    for rule in rulesForLifetimeReweighting[sample]:
        print '\t******'
        print '\tPDG IDs:', rule.pdgIds
        print '\tCTaus:', rule.srcCTaus, '-->', rule.dstCTaus
        print '\tIs default rule:', rule.isDefaultRule
