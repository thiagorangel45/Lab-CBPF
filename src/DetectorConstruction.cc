#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() 
{
    G4bool checkOverlaps = true; 
    G4NistManager* nist = G4NistManager::Instance(); 

    // Volume do mundo
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4double xWorld = 3. * m;
    G4double yWorld = 3. * m;
    G4double zWorld = 3. * m;
    G4Box* solidWorld = new G4Box("World", xWorld, yWorld, zWorld);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));  
    logicWorld->SetVisAttributes(worldVis);

    // Materiais
    G4Material* padMat = nist->FindOrBuildMaterial("G4_Cu");
    G4Material* borderMat = nist->FindOrBuildMaterial("G4_Cu");

    G4double xPad = 14.0 * cm;
    G4double yPad = 18.0 * cm;
    G4double zPad = 0.5 * cm;
    G4double borderThickness = 1.0 * cm;
    G4double cornerSize = 1.0 * cm;

    G4Box* solidPad = new G4Box("Pad", 0.5 * xPad, 0.5 * yPad, 0.5 * zPad);
    G4LogicalVolume* logicPad = new G4LogicalVolume(solidPad, padMat, "logicPad");

    G4Box* solidBorderX = new G4Box("BorderX", 0.5 * borderThickness, 0.5 * yPad, 0.5 * zPad);
    G4Box* solidBorderY = new G4Box("BorderY", 0.5 * xPad, 0.5 * borderThickness, 0.5 * zPad);
    G4LogicalVolume* logicBorderX = new G4LogicalVolume(solidBorderX, borderMat, "logicBorderX");
    G4LogicalVolume* logicBorderY = new G4LogicalVolume(solidBorderY, borderMat, "logicBorderY");

    G4Box* solidCorner = new G4Box("Corner", 0.5 * cornerSize, 0.5 * cornerSize, 0.5 * zPad);
    G4LogicalVolume* logicCorner = new G4LogicalVolume(solidCorner, borderMat, "logicCorner");

    G4VisAttributes* PadVis = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0)); 
    logicPad->SetVisAttributes(PadVis);
    G4VisAttributes* BorderVis = new G4VisAttributes(G4Colour(0, 1, 0)); 
    logicBorderX->SetVisAttributes(BorderVis);
    logicBorderY->SetVisAttributes(BorderVis);
    logicCorner->SetVisAttributes(BorderVis);


    // Posicionamento da 1 Pad
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicPad, "physPad", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(-0.5 * (xPad + borderThickness), 0, 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * (xPad + borderThickness), 0, 0), logicBorderX, "physBorderRight", logicWorld, false, 1, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0, -0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderBottom", logicWorld, false, 2, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0, 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);

    new G4PVPlacement(0, G4ThreeVector(-0.5 * (xPad + borderThickness), -0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBL", logicWorld, false, 4, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * (xPad + borderThickness), -0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(-0.5 * (xPad + borderThickness), 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerTL", logicWorld, false, 6, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * (xPad + borderThickness), 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerTR", logicWorld, false, 7, checkOverlaps);

    for (int i = 1; i < 8; i++)
    {
    new G4PVPlacement(0, G4ThreeVector(0, 0.5 * ((2*i) * yPad + (2*i) * borderThickness), 0), logicPad, "physPad", logicWorld, false, 1, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(-0.5 * (xPad + borderThickness), 0.5 * ((2*i + 1) * yPad + (2*i + 1) * borderThickness), 0), logicCorner, "physCornerTL", logicWorld, false, 6, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * (xPad + borderThickness), 0.5 * ((2*i + 1) * yPad + (2*i + 1) * borderThickness), 0), logicCorner, "physCornerTR", logicWorld, false, 7, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(-0.5 * (xPad + borderThickness), 0.5 * ((2*i) * yPad + (2*i) * borderThickness), 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * (xPad + borderThickness), 0.5 * ((2*i) * yPad + (2*i) * borderThickness), 0), logicBorderX, "physBorderRight", logicWorld, false, 1, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0, 0.5 * ((2*i + 1) * yPad + (2*i + 1) * borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);
    }
    
    for (int j = 1; j < 8; j++)
    {
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j) * xPad + (2*j) * borderThickness), 0, 0), logicPad, "physPad", logicWorld, false, 1, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j) * xPad + (2*j) * borderThickness), 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j + 1) * xPad + (2*j + 1) * borderThickness), 0, 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j) * xPad + (2*j) * borderThickness), -0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderBottom", logicWorld, false, 2, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j + 1)  * xPad + (2*j + 1)  * borderThickness), -0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j + 1)  * xPad + (2*j + 1)  * borderThickness), 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);

    for (int i =0; i < 7; i++){
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j) * xPad + (2*j) * borderThickness), 0.5 * ((2*i + 2) * yPad + (2*i + 2) * borderThickness), 0), logicPad, "physPad", logicWorld, false, 1, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j) * xPad + (2*j) * borderThickness), 0.5 * ((2*i + 3) * yPad + (2*i + 3) * borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j + 1) * xPad + (2*j + 1) * borderThickness), 0.5 * ((2*i + 2) * yPad + (2*i + 2) * borderThickness), 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(0.5 * ((2*j + 1)  * xPad + (2*j + 1)  * borderThickness), 0.5 * ((2*i + 3) * yPad + (2*i + 3) * borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);

    }
    }



    return physWorld;
}
