//segment definitions for the shades, see segmentSet.h in the library for explination of segments

//============================================================================================
//row segments

segmentSection rowSec0[] = {{0, 17}}; //top row, 17 pixels
Segment rowSegment0 = { SIZE(rowSec0), rowSec0, true }; //numSections, section array pointer, direction

segmentSection rowSec1[] = {{17, 17}}; //row 2, 17 pixels, facing reverse direction from top row
Segment rowSegment1 = { SIZE(rowSec1), rowSec1, false }; //numSections, section array pointer, direction

segmentSection rowSec2[] = {{34, 14}}; //row 3, 14 pixels
Segment rowSegment2 = { SIZE(rowSec2), rowSec2, true }; //numSections, section array pointer

segmentSection rowSec3[] = {{48, 14}}; //row 4, 14 pixels, facing reverse direction from top row
Segment rowSegment3 = { SIZE(rowSec3), rowSec3, false }; //numSections, section array pointer

segmentSection rowSec4[] = {{62, 14}}; //row 5, 14 pixels
Segment rowSegment4 = { SIZE(rowSec4), rowSec4, true }; //numSections, section array pointer

Segment *rows_arr[] = { &rowSegment0 , &rowSegment1, &rowSegment2, &rowSegment3, &rowSegment4 };
SegmentSet rowSegments = { SIZE(rows_arr), rows_arr };

//============================================================================================
//row segments lenses (removing the center 3 pixels from the first tow rows)

segmentSection rowLenseSec0[] = {{0, 7}, {10, 7}}; //top row, 14 pixels
Segment rowLenseSegment0 = { SIZE(rowLenseSec0), rowLenseSec0, true }; //numSections, section array pointer, direction

segmentSection rowLenseSec1[] = {{17, 7}, {27, 7}}; //row 2, 14 pixels, facing reverse direction from top row
Segment rowLenseSegment1 = { SIZE(rowLenseSec1), rowLenseSec1, false }; //numSections, section array pointer, direction

Segment *rowLenses_arr[] = { &rowLenseSegment0 , &rowLenseSegment1, &rowSegment2, &rowSegment3, &rowSegment4 };
SegmentSet rowLenseSegments = { SIZE(rowLenses_arr), rowLenses_arr };

//===========================================================================================
//column segments

segmentSection columnsSec0[] = {{0, 1}, {33, 2}, {61, 2} }; //rightmost column: 0, 33, 34, 61, 62
Segment columnsSegment0 = { SIZE(columnsSec0), columnsSec0, true }; //numSections, section array pointer, direction

segmentSection columnsSec1[] = {{1, 1}, {32, 1}, {35, 1}, {60, 1}, {63, 1} }; //column 2: 1, 32, 35, 60, 63
Segment columnsSegment1 = { SIZE(columnsSec1), columnsSec1, true }; //numSections, section array pointer, direction

segmentSection columnsSec2[] = {{2, 1}, {31, 1}, {36, 1}, {59, 1}, {64, 1} }; //column 2: 2, 31, 36, 59, 64
Segment columnsSegment2 = { SIZE(columnsSec2), columnsSec2, true }; //numSections, section array pointer, direction

segmentSection columnsSec3[] = {{3, 1}, {30, 1}, {37, 1}, {58, 1}, {65, 1} }; //column 2: 3, 30, 37, 58, 65
Segment columnsSegment3 = { SIZE(columnsSec3), columnsSec3, true }; //numSections, section array pointer, direction

segmentSection columnsSec4[] = {{4, 1}, {29, 1}, {38, 1}, {57, 1}, {66, 1} }; //column 2: 4, 29, 38, 57, 66
Segment columnsSegment4 = { SIZE(columnsSec4), columnsSec4, true }; //numSections, section array pointer, direction

segmentSection columnsSec5[] = {{5, 1}, {28, 1}, {39, 1}, {56, 1}, {67, 1} }; //column 2: 5, 38, 39, 56, 67
Segment columnsSegment5 = { SIZE(columnsSec5), columnsSec5, true }; //numSections, section array pointer, direction

segmentSection columnsSec6[] = {{6, 1}, {27, 1}, {40, 1}, {55, 1}, {68, 1} }; //column 2: 6, 27, 40, 55, 68
Segment columnsSegment6 = { SIZE(columnsSec6), columnsSec6, true }; //numSections, section array pointer, direction

segmentSection columnsSec7[] = {{7, 1}, {26, 1} }; //column 2: 7, 26
Segment columnsSegment7 = { SIZE(columnsSec7), columnsSec7, true }; //numSections, section array pointer, direction

segmentSection columnsSec8[] = {{8, 1}, {25, 1}}; //column 2: 8, 25
Segment columnsSegment8 = { SIZE(columnsSec8), columnsSec8, true }; //numSections, section array pointer, direction

segmentSection columnsSec9[] = {{9, 1}, {24, 1} }; //column 2: 9, 24
Segment columnsSegment9 = { SIZE(columnsSec9), columnsSec9, true }; //numSections, section array pointer, direction

segmentSection columnsSec10[] = {{10, 1}, {23, 1}, {41, 1}, {54, 1}, {69, 1} }; //column 2: 10, 23, 41, 54, 69
Segment columnsSegment10 = { SIZE(columnsSec10), columnsSec10, true }; //numSections, section array pointer, direction

segmentSection columnsSec11[] = {{11, 1}, {22, 1}, {42, 1}, {53, 1}, {70, 1} }; //column 2: 11, 22, 42, 53, 70
Segment columnsSegment11 = { SIZE(columnsSec11), columnsSec11, true }; //numSections, section array pointer, direction

segmentSection columnsSec12[] = {{12, 1}, {21, 1}, {43, 1}, {52, 1}, {71, 1} }; //column 2: 12, 21, 43, 52, 71
Segment columnsSegment12 = { SIZE(columnsSec12), columnsSec12, true }; //numSections, section array pointer, direction

segmentSection columnsSec13[] = {{13, 1}, {20, 1}, {44, 1}, {51, 1}, {72, 1} }; //column 2: 13, 20, 44, 51, 72
Segment columnsSegment13 = { SIZE(columnsSec13), columnsSec13, true }; //numSections, section array pointer, direction

segmentSection columnsSec14[] = {{14, 1}, {19, 1}, {45, 1}, {50, 1}, {73, 1} }; //column 2: 14, 19, 45, 59, 73
Segment columnsSegment14 = { SIZE(columnsSec14), columnsSec14, true }; //numSections, section array pointer, direction

segmentSection columnsSec15[] = {{15, 1}, {18, 1}, {46, 1}, {49, 1}, {74, 1} }; //column 2: 13, 18, 46, 49, 74
Segment columnsSegment15 = { SIZE(columnsSec15), columnsSec15, true }; //numSections, section array pointer, direction

segmentSection columnsSec16[] = {{16, 2}, {47, 2}, {75, 1} }; //column 2: 16, 17, 47, 48, 75
Segment columnsSegment16 = { SIZE(columnsSec16), columnsSec16, true }; //numSections, section array pointer, direction

Segment *columns_arr[] = { &columnsSegment0 , &columnsSegment1, &columnsSegment2, &columnsSegment3, &columnsSegment4, &columnsSegment5, &columnsSegment6, &columnsSegment7, &columnsSegment8, &columnsSegment9, &columnsSegment10, &columnsSegment11, &columnsSegment12, &columnsSegment13, &columnsSegment14, &columnsSegment15, &columnsSegment16 };
SegmentSet columnSegments = { SIZE(columns_arr), columns_arr };

//=============================================================================================
//row halves, rows start at center
segmentSection rowHalfSec0[] = {{0, 9}}; //top row right, 9 pixels
Segment rowHalfSegment0 = { SIZE(rowSec0), rowHalfSec0, false }; //numSections, section array pointer, direction

segmentSection rowHalfSec1[] = {{8, 9}}; //top row left, 9 pixels
Segment rowHalfSegment1 = { SIZE(rowHalfSec1), rowHalfSec1, true }; //numSections, section array pointer, direction

segmentSection rowHalfSec2[] = {{17, 9}}; //second row left, 9 pixels
Segment rowHalfSegment2 = { SIZE(rowHalfSec2), rowHalfSec2, false }; //numSections, section array pointer, direction

segmentSection rowHalfSec3[] = {{25, 9}}; //second row right, 9 pixels
Segment rowHalfSegment3 = { SIZE(rowHalfSec3), rowHalfSec3, true }; //numSections, section array pointer, direction

segmentSection rowHalfSec4[] = {{34, 7}}; //third row right, 7 pixels
Segment rowHalfSegment4 = { SIZE(rowHalfSec4), rowHalfSec4, false }; //numSections, section array pointer, direction

segmentSection rowHalfSec5[] = {{41, 7}}; //third row left, 7 pixels
Segment rowHalfSegment5 = { SIZE(rowHalfSec5), rowHalfSec5, true }; //numSections, section array pointer, direction

segmentSection rowHalfSec6[] = {{48, 7}}; //forth row left, 7 pixels
Segment rowHalfSegment6 = { SIZE(rowHalfSec6), rowHalfSec6, false }; //numSections, section array pointer, direction

segmentSection rowHalfSec7[] = {{55, 7}}; //forth row right, 7 pixels
Segment rowHalfSegment7 = { SIZE(rowHalfSec7), rowHalfSec7, true }; //numSections, section array pointer, direction

segmentSection rowHalfSec8[] = {{62, 7}}; //fith row right, 7 pixels
Segment rowHalfSegment8 = { SIZE(rowHalfSec8), rowHalfSec8, false }; //numSections, section array pointer, direction

segmentSection rowHalfSec9[] = {{69, 7}}; //fith row left, 7 pixels
Segment rowHalfSegment9 = { SIZE(rowHalfSec9), rowHalfSec9, true }; //numSections, section array pointer, direction

Segment *rowHalves_arr[] = { &rowHalfSegment0,  &rowHalfSegment1,  &rowHalfSegment2,  &rowHalfSegment3,  &rowHalfSegment4, &rowHalfSegment5, &rowHalfSegment6, &rowHalfSegment7, &rowHalfSegment8,  &rowHalfSegment9 };
SegmentSet rowHalvesSegments = { SIZE(rowHalves_arr), rowHalves_arr };

//=============================================================================================
//row lense halves, rows start at center (ie same as row halves, but removing the 3 center pixels from the first two rows)

segmentSection rowLenseHalvesSec0[] = {{0, 7}}; //top row right, 7 pixels
Segment rowLenseHalvesSegment0 = { SIZE(rowLenseHalvesSec0), rowLenseHalvesSec0, false }; //numSections, section array pointer, direction

segmentSection rowLenseHalvesSec1[] = {{10, 7}}; //top row left, 7 pixels
Segment rowLenseHalvesSegment1 = { SIZE(rowLenseHalvesSec1), rowLenseHalvesSec1, true }; //numSections, section array pointer, direction

segmentSection rowLenseHalvesSec2[] = {{17, 7}}; //second row left, 7 pixels
Segment rowLenseHalvesSegment2 = { SIZE(rowLenseHalvesSec2), rowLenseHalvesSec2, false }; //numSections, section array pointer, direction

segmentSection rowLenseHalvesSec3[] = {{27, 7}}; //second row right, 7 pixels
Segment rowLenseHalvesSegment3 = { SIZE(rowLenseHalvesSec3), rowLenseHalvesSec3, true }; //numSections, section array pointer, direction

//the rest of the rows are the same as row halves so:
//Segment *rowHalvesLenses_arr[] = { &rowLenseHalvesSegment0,  &rowLenseHalvesSegment1,  &rowLenseHalvesSegment2,  &rowLenseHalvesSegment3,  &rowHalfSegment4, &rowHalfSegment5, &rowHalfSegment6, &rowHalfSegment7, &rowHalfSegment8,  &rowHalfSegment9 };
//SegmentSet rowLensesHalvesSegments = { SIZE(rowHalvesLenses_arr), rowHalvesLenses_arr };

Segment *rowHalvesLenses_arr[] = { &rowLenseHalvesSegment0,  &rowLenseHalvesSegment2, &rowHalfSegment4, &rowHalfSegment6, &rowHalfSegment8, &rowLenseHalvesSegment1, &rowLenseHalvesSegment3, &rowHalfSegment5, &rowHalfSegment7, &rowHalfSegment9 };
SegmentSet rowLensesHalvesSegments = { SIZE(rowHalvesLenses_arr), rowHalvesLenses_arr };

//=============================================================================================
//solid lenses, ie each side of the glasses is one segment, minus the 6 pixels in the center

segmentSection lenseSec0[] = { {62, 7}, {55, 7}, {34, 7}, {27, 7}, {0, 7} }; //right lense
Segment lenseSegment0 = { SIZE(lenseSec0), lenseSec0, false };

segmentSection lenseSec1[] = {{10, 7}, {17, 7}, {41, 7}, {48, 7}, {69, 7}}; //right lense
Segment lenseSegment1 = { SIZE(lenseSec1), lenseSec1, true };

Segment *lenses_arr[] = { &lenseSegment0 , &lenseSegment1 };
SegmentSet lenseSegments = { SIZE(lenses_arr), lenses_arr };

//=============================================================================================
//solid lenses, ie each side of the glasses is one segment, minus the 6 pixels in the center
//fliped directions from above

segmentSection lenseFSec0[] = {{0, 7}, {27, 7}, {34, 7}, {55, 7}, {62, 7}}; //right lense
Segment lenseFSegment0 = { SIZE(lenseFSec0), lenseFSec0, true };

segmentSection lenseFSec1[] = {{69, 7}, {48, 7}, {41, 7}, {17, 7}, {10, 7}}; //right lense
Segment lenseFSegment1 = { SIZE(lenseFSec1), lenseFSec1, false };

Segment *lensesF_arr[] = { &lenseFSegment0 , &lenseFSegment1 };
SegmentSet lenseFSegments = { SIZE(lensesF_arr), lensesF_arr };
