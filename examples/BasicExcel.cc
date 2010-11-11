#include <stdio.h>
#include <BasicExcel.hpp>
#include <assert.h>

int main(int argc, char **argv)
{
  YExcel::BasicExcel e;
  e.New(1);
  e.RenameWorksheet(size_t(0),"BasicExcel");
  YExcel::BasicExcelWorksheet* sheet = e.GetWorksheet(size_t(0));
  assert(sheet);
  sheet->Cell(0,0)->SetString("A");
  sheet->Cell(0,1)->SetString("B");
  sheet->Cell(0,1)->SetStyle(YExcel::BasicExcelCell::ST_YELLOW_BG);
  sheet->Cell(1,0)->SetString("C");
  sheet->Cell(1,0)->SetStyle(YExcel::BasicExcelCell::ST_BOLD);
  sheet->Cell(1,1)->SetString("D");
  sheet->Cell(1,1)->SetStyle(YExcel::BasicExcelCell::ST_DBL_ULINE);
  e.SaveAs("BasicExcel.xls");
  return 0;
}
