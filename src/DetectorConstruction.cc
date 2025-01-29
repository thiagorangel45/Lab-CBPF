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

    // Dimensões das Pads e dos espaçadores
    G4double xPad = 14.0 * cm;
    G4double yPad = 18.0 * cm;
    G4double zPad = 0.5 * cm;

    G4double xSpacer = 1.0 * cm;
    G4double ySpacer = 1.0 * cm;
    G4double zSpacer = 0.5 * cm;

    G4Material* padMat = nist->FindOrBuildMaterial("G4_Cu");
    G4Material* spacerMat = nist->FindOrBuildMaterial("G4_Li");

    // Criar as Pads e Spacers
    G4Box *solidPad = new G4Box("Pad", 0.5 * xPad, 0.5 * yPad, 0.5 * zPad);
    G4LogicalVolume *logicPad = new G4LogicalVolume(solidPad, padMat, "logicPad");

    G4Box *solidSpacerX = new G4Box("SpacerX", 0.5 * xSpacer, 0.5 * yPad, 0.5 * zSpacer);
    G4LogicalVolume *logicSpacerX = new G4LogicalVolume(solidSpacerX, spacerMat, "logicSpacerX");

    G4Box *solidSpacerY = new G4Box("SpacerY", 0.5 * xPad, 0.5 * ySpacer, 0.5 * zSpacer);
    G4LogicalVolume *logicSpacerY = new G4LogicalVolume(solidSpacerY, spacerMat, "logicSpacerY");

    // Cores
    G4VisAttributes* PadVis = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0)); // Laranja
    logicPad->SetVisAttributes(PadVis);
    G4VisAttributes* SpacerVis = new G4VisAttributes(G4Colour(0, 1, 0)); // Verde
    logicSpacerX->SetVisAttributes(SpacerVis);
    logicSpacerY->SetVisAttributes(SpacerVis);

    // Dimensões totais do detector
    G4double totalWidth  = 8 * xPad + 7 * xSpacer;
    G4double totalHeight = 8 * yPad + 7 * ySpacer;

    // Ajuste para centralizar no mundo
    G4double xStart = -0.5 * totalWidth;
    G4double yStart = -0.5 * totalHeight;
    G4double zStart = 1.0 * cm;  // Mantendo na altura original

    // Criar matriz 8x8
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Posição das Pads centralizada
            G4double xPos = xStart + i * (xPad + xSpacer);
            G4double yPos = yStart + j * (yPad + ySpacer);
            G4ThreeVector padPosition(xPos, yPos, zStart);
            new G4PVPlacement(0, padPosition, logicPad, "physPad", logicWorld, false, i * 8 + j, checkOverlaps);

            // Adicionar espaçadores horizontais (entre Pads no eixo X)
            if (i < 7) {
                G4ThreeVector spacerXPos(xPos + 0.5 * xPad + 0.5 * xSpacer, yPos, zStart);
                new G4PVPlacement(0, spacerXPos, logicSpacerX, "physSpacerX", logicWorld, false, i * 8 + j, checkOverlaps);
            }

            // Adicionar espaçadores verticais (entre Pads no eixo Y)
            if (j < 7) {
                G4ThreeVector spacerYPos(xPos, yPos + 0.5 * yPad + 0.5 * ySpacer, zStart);
                new G4PVPlacement(0, spacerYPos, logicSpacerY, "physSpacerY", logicWorld, false, i * 8 + j, checkOverlaps);
            }
        }
    }

    return physWorld;
}
