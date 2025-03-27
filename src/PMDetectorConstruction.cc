#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;
    G4NistManager *nist  = G4NistManager::Instance();
    
    // Defindo a fonte radiotiva
    G4Isotope *F18 = new G4Isotope("F18", 9, 18, 18.000938 * g / mole);
    G4Element *elF18 = new G4Element("Fluorine-18", "F18", 1);
    elF18->AddIsotope(F18, 100.0 * perCent);
    G4Material *matF18 = new G4Material("F18Source", 1.51 * g / cm3, 1);
    matF18->AddElement(elF18, 100.0 * perCent); 

    G4double sourceRadius = 1. * mm; // raio da fonte de radiação
    G4Sphere *solidSource = new G4Sphere("solidSource", 0.0, sourceRadius, 0.0, 360. * deg, 0.0, 180. * deg);
    G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, matF18, "logicSource");
    G4VisAttributes *sourceVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 1.0, 0.5));
    sourceVisAtt->SetForceSolid(true);
    logicSource->SetVisAttributes(sourceVisAtt);

    // definindo o mundo
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4double xWorld = 3. * m;
    G4double yWorld = 3. * m;
    G4double zWorld = 3. * m;

    G4Box* solidWorld = new G4Box("World", xWorld, yWorld, zWorld);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); 
    logicWorld->SetVisAttributes(worldVis);


    // materiais do detector
    G4Material* padMat = nist->FindOrBuildMaterial("G4_Cu");
    G4Material* borderMat = nist->FindOrBuildMaterial("G4_Cu"); // por hora colocando o mesmo material (alterar depois)
    // continuar com os outros materiais ......

    // tamanho das Pads
    G4double xPad = 14.0 * cm;
    G4double yPad = 18.0 * cm;
    G4double zPad = 0.5 * cm;
    G4double borderThickness = 1.0 * cm;
    G4double cornerSize = 1.0 * cm;

    // determinando a Pad
    G4Box* solidPad = new G4Box("Pad", 0.5 * xPad, 0.5 * yPad, 0.5 * zPad);
    logicPad = new G4LogicalVolume(solidPad, padMat, "logicPad");
    G4VisAttributes* PadVis = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0)); 
    logicPad->SetVisAttributes(PadVis);
    G4VisAttributes* BorderVis = new G4VisAttributes(G4Colour(0, 1, 0)); 

    // determinando a borda X
    G4Box* solidBorderX = new G4Box("BorderX", 0.5 * borderThickness, 0.5 * yPad, 0.5 * zPad);
    G4LogicalVolume* logicBorderX = new G4LogicalVolume(solidBorderX, borderMat, "logicBorderX");
    logicBorderX->SetVisAttributes(BorderVis);

    // determinando a borda Y
    G4Box* solidBorderY = new G4Box("BorderY", 0.5 * xPad, 0.5 * borderThickness, 0.5 * zPad);
    G4LogicalVolume* logicBorderY = new G4LogicalVolume(solidBorderY, borderMat, "logicBorderY");
    logicBorderY->SetVisAttributes(BorderVis);

    // determinando o contorno
    G4Box* solidCorner = new G4Box("Corner", 0.5 * cornerSize, 0.5 * cornerSize, 0.5 * zPad);
    G4LogicalVolume* logicCorner = new G4LogicalVolume(solidCorner, borderMat, "logicCorner");
    logicCorner->SetVisAttributes(BorderVis);

   // Definição do offset para centralizar a matriz
    G4double offsetX = -3.5 * (xPad + borderThickness);
    G4double offsetY = -3.5 * (yPad + borderThickness);

    // Posicionamento da 1 Pad
    new G4PVPlacement(0, G4ThreeVector(offsetX, offsetY, 0), logicPad, "physPad", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX - 0.5 * (xPad + borderThickness), offsetY, 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX + 0.5 * (xPad + borderThickness), offsetY, 0), logicBorderX, "physBorderRight", logicWorld, false, 1, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX, offsetY - 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderBottom", logicWorld, false, 2, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX, offsetY + 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);

    new G4PVPlacement(0, G4ThreeVector(offsetX - 0.5 * (xPad + borderThickness), offsetY - 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBL", logicWorld, false, 4, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX + 0.5 * (xPad + borderThickness), offsetY - 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX - 0.5 * (xPad + borderThickness), offsetY + 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerTL", logicWorld, false, 6, checkOverlaps);
    new G4PVPlacement(0, G4ThreeVector(offsetX + 0.5 * (xPad + borderThickness), offsetY + 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerTR", logicWorld, false, 7, checkOverlaps);

    for (int i = 1; i < 8; i++)
    {
        G4double posY = offsetY + 0.5 * ((2*i) * yPad + (2*i) * borderThickness);
        new G4PVPlacement(0, G4ThreeVector(offsetX, posY, 0), logicPad, "physPad", logicWorld, false, 1, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(offsetX - 0.5 * (xPad + borderThickness), posY + 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerTL", logicWorld, false, 6, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(offsetX + 0.5 * (xPad + borderThickness), posY + 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerTR", logicWorld, false, 7, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(offsetX - 0.5 * (xPad + borderThickness), posY, 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(offsetX + 0.5 * (xPad + borderThickness), posY, 0), logicBorderX, "physBorderRight", logicWorld, false, 1, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(offsetX, posY + 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);
    }

    for (int j = 1; j < 8; j++)
    {
        G4double posX = offsetX + 0.5 * ((2*j) * xPad + (2*j) * borderThickness);
        new G4PVPlacement(0, G4ThreeVector(posX, offsetY, 0), logicPad, "physPad", logicWorld, false, 1, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(posX, offsetY + 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(posX + 0.5 * (xPad + borderThickness), offsetY, 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(posX, offsetY - 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderBottom", logicWorld, false, 2, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(posX + 0.5 * (xPad + borderThickness), offsetY - 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(posX + 0.5 * (xPad + borderThickness), offsetY + 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);

        for (int i = 0; i < 7; i++)
        {
            G4double posY = offsetY + 0.5 * ((2*i + 2) * yPad + (2*i + 2) * borderThickness);
            new G4PVPlacement(0, G4ThreeVector(posX, posY, 0), logicPad, "physPad", logicWorld, false, 1, checkOverlaps);
            new G4PVPlacement(0, G4ThreeVector(posX, posY + 0.5 * (yPad + borderThickness), 0), logicBorderY, "physBorderTop", logicWorld, false, 3, checkOverlaps);
            new G4PVPlacement(0, G4ThreeVector(posX + 0.5 * (xPad + borderThickness), posY, 0), logicBorderX, "physBorderLeft", logicWorld, false, 0, checkOverlaps);
            new G4PVPlacement(0, G4ThreeVector(posX + 0.5 * (xPad + borderThickness), posY + 0.5 * (yPad + borderThickness), 0), logicCorner, "physCornerBR", logicWorld, false, 5, checkOverlaps);
        }
    }
    
    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitiveDetector");
    logicPad->SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}