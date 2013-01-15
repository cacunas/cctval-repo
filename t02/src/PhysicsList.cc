#include "globals.hh"
#include "PhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

//- Fix for Scintillator
#include "G4PhysicsListHelper.hh" //-
#include "G4GammaConversionToMuons.hh" //-
#include "G4eplusAnnihilation.hh" //-
#include "G4OpAbsorption.hh" //-
#include "G4OpRayleigh.hh" //-
#include "G4OpBoundaryProcess.hh" //-
#include "G4Scintillation.hh" //-

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4ionIonisation.hh"
#include "G4Decay.hh"

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

PhysicsList::PhysicsList() : G4VPhysicsList()
{
    defaultCutValue = 1.0 * cm;
    SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::ConstructParticle()
{
    ConstructBosons();
    ConstructLeptons();
    ConstructMesons();
    ConstructBaryons();
}

void PhysicsList::ConstructBosons()
{
    G4Gamma::GammaDefinition();
    G4OpticalPhoton::OpticalPhotonDefinition(); //-
}

void PhysicsList::ConstructLeptons()
{
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();

    G4MuonPlus::MuonPlusDefinition();
    G4MuonMinus::MuonMinusDefinition();

    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();

    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void PhysicsList::ConstructMesons()
{
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4PionZero::PionZeroDefinition();
    G4Eta::EtaDefinition();
    G4EtaPrime::EtaPrimeDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonMinus::KaonMinusDefinition();
    G4KaonZero::KaonZeroDefinition();
    G4AntiKaonZero::AntiKaonZeroDefinition();
    G4KaonZeroLong::KaonZeroLongDefinition();
    G4KaonZeroShort::KaonZeroShortDefinition();
}

void PhysicsList::ConstructBaryons()
{
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();

    G4Neutron::NeutronDefinition();
    G4AntiNeutron::AntiNeutronDefinition();
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();
    ConstructEM();
    ConstructOp();
    ConstructGeneral();
    ConstructScintillation();
    AddStepMax();
}

void PhysicsList::ConstructEM()
{
    theParticleIterator->reset();
    while ((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {
            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
            pmanager->AddDiscreteProcess(new G4ComptonScattering);
            pmanager->AddDiscreteProcess(new G4GammaConversion);

        } else if (particleName == "e-") {
            pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);

        } else if (particleName == "e+") {
            pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation, 0, -1, 4);

        } else if (particleName == "mu+" ||
                particleName == "mu-") {
            pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4MuIonisation, -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung, -1, 3, 3);
            pmanager->AddProcess(new G4MuPairProduction, -1, 4, 4);

        } else if (particleName == "proton" ||
                particleName == "pi-" ||
                particleName == "pi+") {
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation, -1, 2, 2);
            pmanager->AddProcess(new G4hBremsstrahlung, -1, 3, 3);
            pmanager->AddProcess(new G4hPairProduction, -1, 4, 4);

        } else if (particleName == "alpha" ||
                particleName == "He3") {
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4ionIonisation, -1, 2, 2);

        } else if (particleName == "GenericIon") {
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4ionIonisation, -1, 2, 2);

        } else if ((!particle->IsShortLived()) &&
                (particle->GetPDGCharge() != 0.0)) {
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation, -1, 2, 2);
        }
    }
}

void PhysicsList::ConstructOp() //-
{

    G4PhysicsListHelper* helper = G4PhysicsListHelper::GetPhysicsListHelper();
    G4ParticleDefinition* OpPhoton;

    OpPhoton = G4OpticalPhoton::OpticalPhoton();
    helper->RegisterProcess(new G4OpBoundaryProcess(), OpPhoton);
    helper->RegisterProcess(new G4OpAbsorption(), OpPhoton);
    helper->RegisterProcess(new G4OpRayleigh(), OpPhoton);

}

void PhysicsList::ConstructScintillation() //-
{

    G4Scintillation* Scint = new G4Scintillation("Scintillation");
    Scint->SetTrackSecondariesFirst(true);

    G4PhysicsListHelper* helper = G4PhysicsListHelper::GetPhysicsListHelper();
    theParticleIterator->reset();
    while ((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        if (Scint->IsApplicable(*particle)) 
            helper->RegisterProcess(Scint, particle);
    }

}

void PhysicsList::ConstructGeneral()
{
    G4Decay* theDecayProcess = new G4Decay();
    theParticleIterator->reset();
    while ((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (theDecayProcess->IsApplicable(*particle)) {
            pmanager ->AddProcess(theDecayProcess);
            pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
    }
}

void PhysicsList::AddStepMax()
{
    G4StepLimiter* stepLimiter = new G4StepLimiter();
    theParticleIterator->reset();
    while ((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        if (particle->GetPDGCharge() != 0.0) {
            pmanager ->AddDiscreteProcess(stepLimiter);
        }
    }
}

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
    if (verboseLevel > 0) DumpCutValuesTable();
}
