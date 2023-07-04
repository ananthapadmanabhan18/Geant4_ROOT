#include "construction.hh"
#include "math.h"
#include "detector.hh"

scintillation1::scintillation1()
{}
scintillation1::~scintillation1()
{}
G4VPhysicalVolume *scintillation1::Construct()
{
    // Set up the dimensions of the honey comb detector
    G4double phiStart = - 0 * M_PI;
    G4double phiTotal =  2* M_PI; 
    G4int numZplanes = 2;
    G4int numSides=6;
    G4double rInner[]= {2.4*mm, 2.4*mm};
    G4double rOuter[] = {2.5*mm, 2.5*mm};
    G4double zPlanes[]={-2.5*mm,2.5*mm};
    G4double gInner[]= {0.01*mm, 0.01*mm};
    G4double gOuter[] = {2.4*mm, 2.4*mm};

    G4RotationMatrix* rotmat = new G4RotationMatrix;
    rotmat->rotateZ(90*deg);

    G4double atomicNumber = 29;
    G4double atomicMass = 63.55 * g/mole;
    G4double density1 = 8.96 * g/cm3;

    // Define materials
    G4NistManager* nistManager = G4NistManager::Instance();
    //Defining Elements
    G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
    G4Material* plastic = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Element* gold = nistManager->FindOrBuildElement("Au");
    G4Element* copper = nistManager->FindOrBuildElement("Cu");
    G4Element* argonGas = nistManager->FindOrBuildElement("Ar");
    G4Element* Aluminium = nistManager->FindOrBuildElement("Al");
    //Aluminium
    G4Material* Al = new G4Material("Aluminium", 2.7 * g/cm3, 1);
    Al->AddElement(Aluminium,1);    
    //gold material
    G4Material* Au = new G4Material("Gold", 19.32 * g/cm3, 1);
    Au->AddElement(gold,1);
    //copper material
    G4Material* Cu = new G4Material("Copper", 8.96 * g/cm3, 1);
    Cu->AddElement(copper,1);
    //Carbon Monoxide
    G4double density = 1.25*g/L;
    G4Material* CO = new G4Material("CO",1.25*g/L,2);
    CO->AddElement(nistManager->FindOrBuildElement("C"), 1);
    CO->AddElement(nistManager->FindOrBuildElement("O"), 1);
    //Gas Mixture
    G4Material* gasMixture = new G4Material("GasMixture", 1.72 , 2);
    gasMixture->AddElement(argonGas, 0.9);
    gasMixture->AddMaterial(CO, 0.1);



    //Defining colours
    G4VisAttributes* vishc = new G4VisAttributes(G4Colour(1,0.72,0.243));
    vishc->SetForceSolid(true);

    G4VisAttributes* visgold = new G4VisAttributes(G4Colour(1,0.843,0));
    visgold->SetForceSolid(true);

    G4VisAttributes* visscintilation = new G4VisAttributes(G4Colour(0,0,1));
    visscintilation->SetForceSolid(true);

    G4VisAttributes* gas = new G4VisAttributes(G4Colour(0.85,0.85,1,0.3));
    gas->SetForceSolid(true);

    G4VisAttributes* WW = new G4VisAttributes(G4Colour(0.85,0.85,1,0));
    WW->SetForceSolid(true);

    G4bool checkoverlap = false;

    //Defining World Volume
    G4Box *solidworld = new G4Box("world",2.5*m,2.5*m,2.5*m);
    G4LogicalVolume *logicworld = new G4LogicalVolume(solidworld, air,"logicalworld");
    G4VPhysicalVolume *physicalworld = new G4PVPlacement(nullptr,G4ThreeVector(0,0,0),logicworld, "physicalworld", nullptr, false,0,checkoverlap);






    //Defining 2 Scintillation Detectors Detector
    G4Box* solidDetector = new G4Box("Detector1", 1.15*cm,11.25*cm,0.5*cm);
    G4LogicalVolume* logicalDetector = new G4LogicalVolume(solidDetector, plastic, "Detector1");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0*m, -11.5*cm), logicalDetector, "Detector1", logicworld, false, 0,checkoverlap);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0*m,11.5*cm), logicalDetector, "Detector2", logicworld, false, 0,checkoverlap);
    
    
    

    //Defining Honeycomb detector
    G4Polyhedra *hcdetector = new G4Polyhedra("HCD", phiStart,phiTotal,numSides,numZplanes,zPlanes,rInner, rOuter);
    G4LogicalVolume* hclogic = new G4LogicalVolume(hcdetector,Cu,"hclogic");
    hclogic->SetVisAttributes(vishc);



    //Defining the Gold wire 
    G4Tubs *goldwire = new G4Tubs("goldwire",0,0.01*mm,2.5*mm,0,2*M_PI);
    G4LogicalVolume* goldlogic = new G4LogicalVolume(goldwire,Au,"goldlogic");
    goldlogic->SetVisAttributes(visgold);


    //Filling the detector with argon/CO mixture
    G4Polyhedra *gassfill = new G4Polyhedra("gas", phiStart,phiTotal,numSides,numZplanes,zPlanes,gInner, gOuter);
    G4LogicalVolume* gaslogic = new G4LogicalVolume(gassfill,air,"gaslogic");
    gaslogic->SetVisAttributes(gas);
    
    
    

    //Placing the logic volumes of HoneyComb
    for (int i = -23;i<=24;i++){
        for (int j = -23;j<=24;j++){

            new G4PVPlacement(nullptr, G4ThreeVector(i*(15/sqrt(3))*mm,j*(5)*mm,0), hclogic, "HCdetector", logicworld, false, 0,checkoverlap);
            new G4PVPlacement(nullptr, G4ThreeVector((i*(15/sqrt(3))-(7.5/sqrt(3)))*mm,((j*5)-(2.5))*mm,0), hclogic, "HCdetector", logicworld, false,j+i*100,checkoverlap);

            // new G4PVPlacement(nullptr, G4ThreeVector(i*(15/sqrt(3))*mm,j*(5)*mm,0), goldlogic, "goldwire", logicworld, false, 0,checkoverlap);
            // new G4PVPlacement(nullptr, G4ThreeVector((i*(15/sqrt(3))-(7.5/sqrt(3)))*mm,((j*5)-(2.5))*mm,0), goldlogic, "goldwire", logicworld, false, j+i*100,checkoverlap);


            // new G4PVPlacement(nullptr, G4ThreeVector(i*(15/sqrt(3))*mm,j*(5)*mm,0), gaslogic, "gasmix", logicworld, false, i+j*100,checkoverlap);
            // new G4PVPlacement(nullptr, G4ThreeVector((i*(15/sqrt(3))-(7.5/sqrt(3)))*mm,((j*5)-(2.5))*mm,0), gaslogic, "gasmix", logicworld, false, j+i*100,checkoverlap);
        }



    // //The Round things
    // G4Tubs *round1 = new G4Tubs("round",0,200*mm,20*cm,0,2*M_PI);
    // G4LogicalVolume* round1logic = new G4LogicalVolume(round1,plastic,"roundlogic");
    // new G4PVPlacement(rotmat, G4ThreeVector(50*cm,50*cm,50*cm), round1logic, "bleh", logicworld, false, 0,checkoverlap);





    //Defining the PCB
    G4Box* pcb = new G4Box("pcb", ((15/sqrt(3))*24+(7.5/sqrt(3)))*mm,(12.5)*cm,1.25*mm);
    G4LogicalVolume* pcblogic = new G4LogicalVolume(pcb, plastic, "Detector1");
    new G4PVPlacement(nullptr, G4ThreeVector(-15/sqrt(3)*0*mm, -5*0*mm, -3.75*mm), pcblogic, "Detector1", logicworld, false, 0,checkoverlap);
    new G4PVPlacement(nullptr, G4ThreeVector(-15/sqrt(3)*0*mm, -5*0*mm,3.75*mm), pcblogic, "Detector2", logicworld, false, 0,checkoverlap);






    //Defining the Stand  
    G4double phitotalstand=M_PI;
    G4int standsidenum=2;
    G4int planestandno =2;
    G4double zPlanestand[] = {-15*cm,15*cm};
    G4double standouter[]={1.25*cm,1.25*cm};
    G4double standinner[]={0.75*cm,0.75*cm};

    G4Polyhedra *stand1 = new G4Polyhedra("HCD", -45*deg,phitotalstand,standsidenum,planestandno,zPlanestand,standinner, standouter);
    G4LogicalVolume* stand1logic = new G4LogicalVolume(stand1,Al,"stand");
    new G4PVPlacement(nullptr, G4ThreeVector(((36.75/sqrt(3)-1.25))*cm,11.25*cm,-15.5*cm), stand1logic, "stand", logicworld, false, 0,checkoverlap);

    G4Polyhedra *stand2 = new G4Polyhedra("HCD", 45*deg,phitotalstand,standsidenum,planestandno,zPlanestand,standinner, standouter);
    G4LogicalVolume* stand2logic = new G4LogicalVolume(stand2,Al,"stand");    
    new G4PVPlacement(nullptr, G4ThreeVector(-((36.75/sqrt(3)-1.25))*cm,11.25*cm,-15.5*cm), stand2logic, "stand", logicworld, false, 0,checkoverlap);


    G4Polyhedra *stand3 = new G4Polyhedra("HCD", 135*deg,phitotalstand,standsidenum,planestandno,zPlanestand,standinner, standouter);
    G4LogicalVolume* stand3logic = new G4LogicalVolume(stand3,Al,"stand");
    new G4PVPlacement(nullptr, G4ThreeVector(-((36.75/sqrt(3)-1.25))*cm,-11.25*cm,-15.5*cm), stand3logic, "stand", logicworld, false, 0,checkoverlap);

    G4Polyhedra *stand4 = new G4Polyhedra("HCD", 225*deg,phitotalstand,standsidenum,planestandno,zPlanestand,standinner, standouter);
    G4LogicalVolume* stand4logic = new G4LogicalVolume(stand4,Al,"stand");    
    new G4PVPlacement(nullptr, G4ThreeVector(((36.75/sqrt(3)-1.25))*cm,-11.25*cm,-15.5*cm), stand4logic, "stand", logicworld, false, 0,checkoverlap);














    }
 

    //Assigning the Sensitive detectors for the gas volume
    GSD* sensitiveDetector = new GSD("MySensitiveDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);
    gaslogic->SetSensitiveDetector(sensitiveDetector);
    return physicalworld;  
}

