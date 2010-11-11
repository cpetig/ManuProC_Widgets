#include <stdio.h>
#include <string>
#include <iostream>
#include <BasicExcel.hpp>
#include <assert.h>
#include <Misc/itos.h>

static std::string iso2utf8(const std::string &s)
{  std::string ret="";

   for (std::string::const_iterator i = s.begin(); i!=s.end() ; i++)
   {  if ((unsigned char)(*i)>127)
      {  ret+=(unsigned char)(0xc0|(((unsigned char)(*i))>>6));
         ret+=(unsigned char)(0x80|((*i)&0x3f));
      }
      else ret+=*i;
   }
   return ret;
}

// a hack? ... but seems to be the most efficient
static std::string un_wide(std::wstring const& x)
{
  char res[10240];
  size_t r= wcstombs(res,x.c_str(),sizeof(res));
  if (r==(size_t)(-1)) return std::string();
  return res;
}

static std::string to_text(YExcel::BasicExcelCell const &c)
{
  switch (c.Type())
  {
    case YExcel::BasicExcelCell::DOUBLE:
      return dtos(c.GetDouble());
    case YExcel::BasicExcelCell::INT:
      return itos(c.GetInteger());
    case YExcel::BasicExcelCell::STRING:
      return iso2utf8(c.GetString());
    case YExcel::BasicExcelCell::WSTRING:
      return un_wide(c.GetWString());
    default:
      return std::string();
  }
}

int main(int argc, char **argv)
{
  assert(setlocale(LC_ALL/*CTYPE*/, ""));
  assert(setlocale(LC_CTYPE, ""));

  if (argc!=2)
  { fprintf(stderr, "USAGE: %s <excel_file>\n", argv[0]); abort(); }

  YExcel::BasicExcel e;
  assert(e.Load(argv[1]));
  YExcel::BasicExcelWorksheet* sheet = e.GetWorksheet(size_t(0));
  assert(sheet);
  // minimal header
  for (unsigned i=0; i<sheet->GetTotalRows(); ++i)
  for (unsigned j=0; j<sheet->GetTotalCols(); ++j)
  {
    if (sheet->Cell(i,j)->Type()!=YExcel::BasicExcelCell::UNDEFINED)
      std::cout << i << ',' << j << ' ' <<  to_text(*sheet->Cell(i,j)) << '\n';
  }
  return 0;
}
