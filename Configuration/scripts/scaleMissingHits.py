import os

dataDir = '/abyss/users/mcarrigan/MissingHitsCorrections/2023_postBpix/'

cmd = f'hadd -f {dataDir}/MissingHits_2023D.root '

for this_dir in os.listdir(dataDir):
    if not os.path.isdir('/'.join([dataDir, this_dir])): continue
    #if 'WtoLNu-4Jets' in this_dir:
    #    dataset = 'WToLNu_4Jets_2022'
    #else:
    print(this_dir)
    dataset = this_dir.replace('_bpix', 'PostBPix') 
    dataset = dataset.replace('MissingHits_', '')
    fullPath = '/'.join([dataDir, this_dir])
    #cmd = f'python3 mergeOut.py --dir {fullPath} -f background_MC --targetLumi MET_2023D -d {dataset}'
    #print(cmd)
    #os.system(cmd)
    cmd += f' {dataDir}/{this_dir}/{dataset}.root'

print(cmd)
os.system(cmd)
