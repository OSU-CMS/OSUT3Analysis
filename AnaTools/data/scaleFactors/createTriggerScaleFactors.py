import ROOT

electronTrigger2016ee = ROOT.TH2D("electronTrigger2016ee", "electronTrigger2016ee", 1, 0, 500, 1, 0, 1.5)
electronTrigger2016ee.SetTitle("electronTrigger2016ee;pt [GeV];|#eta|")
electronTrigger2016ee.SetBinContent(1,1,1.0)

muonTrigger2016mumu = ROOT.TH2D("muonTrigger2016mumu", "muonTrigger2016mumu", 1, 0, 500, 1, 0, 1.5)
muonTrigger2016mumu.SetTitle("muonTrigger2016mumu;pt [GeV];|#eta|")
muonTrigger2016mumu.SetBinContent(1,1,0.956)

electronTrigger2016emu = ROOT.TH2D("electronTrigger2016emu", "electronTrigger2016emu", 1, 0, 500, 1, 0, 1.5)
electronTrigger2016emu.SetTitle("electronTrigger2016emu;pt [GeV];|#eta|")
electronTrigger2016emu.SetBinContent(1,1,0.974)

muonTrigger2016emu = ROOT.TH2D("muonTrigger2016emu", "muonTrigger2016emu", 1, 0, 500, 1, 0, 1.5)
muonTrigger2016emu.SetTitle("muonTrigger2016emu;pt [GeV];|#eta|")
muonTrigger2016emu.SetBinContent(1,1,0.975)

############

electronTrigger2017ee = ROOT.TH2D("electronTrigger2017ee", "electronTrigger2017ee", 1, 0, 500, 1, 0, 1.5)
electronTrigger2017ee.SetTitle("electronTrigger2017ee;pt [GeV];|#eta|")
electronTrigger2017ee.SetBinContent(1,1,1.0)

muonTrigger2017mumu = ROOT.TH2D("muonTrigger2017mumu", "muonTrigger2017mumu", 1, 0, 500, 1, 0, 1.5)
muonTrigger2017mumu.SetTitle("muonTrigger2017mumu;pt [GeV];|#eta|")
muonTrigger2017mumu.SetBinContent(1,1,0.930)

electronTrigger2017emu = ROOT.TH2D("electronTrigger2017emu", "electronTrigger2017emu", 1, 0, 500, 1, 0, 1.5)
electronTrigger2017emu.SetTitle("electronTrigger2017emu;pt [GeV];|#eta|")
electronTrigger2017emu.SetBinContent(1,1,0.961)

muonTrigger2017emu = ROOT.TH2D("muonTrigger2017emu", "muonTrigger2017emu", 1, 0, 500, 1, 0, 1.5)
muonTrigger2017emu.SetTitle("muonTrigger2017emu;pt [GeV];|#eta|")
muonTrigger2017emu.SetBinContent(1,1,0.972)

############

electronTrigger2018ee = ROOT.TH2D("electronTrigger2018ee", "electronTrigger2018ee", 1, 0, 500, 1, 0, 1.5)
electronTrigger2018ee.SetTitle("electronTrigger2018ee;pt [GeV];|#eta|")
electronTrigger2018ee.SetBinContent(1,1,1.0)

muonTrigger2018mumu = ROOT.TH2D("muonTrigger2018mumu", "muonTrigger2018mumu", 1, 0, 500, 1, 0, 1.5)
muonTrigger2018mumu.SetTitle("muonTrigger2018mumu;pt [GeV];|#eta|")
muonTrigger2018mumu.SetBinContent(1,1,0.935)

electronTrigger2018emu = ROOT.TH2D("electronTrigger2018emu", "electronTrigger2018emu", 1, 0, 500, 1, 0, 1.5)
electronTrigger2018emu.SetTitle("electronTrigger2018emu;pt [GeV];|#eta|")
electronTrigger2018emu.SetBinContent(1,1,0.965)

muonTrigger2018emu = ROOT.TH2D("muonTrigger2018emu", "muonTrigger2018emu", 1, 0, 500, 1, 0, 1.5)
muonTrigger2018emu.SetTitle("muonTrigger2018emu;pt [GeV];|#eta|")
muonTrigger2018emu.SetBinContent(1,1,0.967)

############


electronFile = ROOT.TFile( 'electronTriggerScaleFactors.root', 'RECREATE' )

electronTrigger2016ee.Write()
electronTrigger2016emu.Write()

electronTrigger2017ee.Write()
electronTrigger2017emu.Write()

electronTrigger2018ee.Write()
electronTrigger2018emu.Write()

electronFile.Close()


muonFile = ROOT.TFile( 'muonTriggerScaleFactors.root', 'RECREATE' )

muonTrigger2016mumu.Write()
muonTrigger2016emu.Write()

muonTrigger2017mumu.Write()
muonTrigger2017emu.Write()

muonTrigger2018mumu.Write()
muonTrigger2018emu.Write()

muonFile.Close()
