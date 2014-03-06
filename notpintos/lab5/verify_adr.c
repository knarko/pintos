#include <stdlib.h>
#include "pagedir.h"


/* Kontrollera alla adresser från och med start till och inte med
 * (start+length). */
bool verify_fix_length(void* start, int length)
{
  void* adr;
  void* end = start+length;

  for (adr = pg_round_down(start); adr < end; adr += PGSIZE)
    {
      if(pagedir_get_page(NULL, adr) == NULL)
	return false;
    }
  return true;
}

/* Kontrollera alla adresser från och med start till och med den
 * adress som först inehåller ett noll-tecken, `\0'. (C-strängar
 * lagras på detta sätt.) */
bool verify_variable_length(char* start)
{
  /*  if (pagedir_get_page(NULL, start) == NULL)
    return false;

  char* adr;
  for(adr = start; !is_end_of_string(adr);) 
    {
    ++adr;
    if (pagedir_get_page(NULL, adr) == NULL)
      {
	return false;
      }
    }

  return true;
  */
  
  char* adr = start;
  void* page_adr = pg_round_down(adr);

  if (pagedir_get_page(NULL, page_adr) == NULL) 
    return false;

  if (is_end_of_string(adr))
    return true;

  page_adr += PGSIZE;

  do {
    ++adr;
    if (adr == page_adr)      
      {
	printf("before: %p",page_adr);
	if (pagedir_get_page(NULL, page_adr) == NULL)
	  {
	    return false;
	  }
	page_adr += PGSIZE;
	printf("after: %p",page_adr);
      }
  } while (!is_end_of_string(adr));

  return true;
}

/* Definition av testfall */
struct test_case_t
{
  void* start;
  unsigned length;
};

#define TEST_CASE_COUNT 6

const struct test_case_t test_case[TEST_CASE_COUNT] =
  {
    {(void*)100, 100}, /* one full page */
    {(void*)199, 102},
    {(void*)101, 98},
    {(void*)250, 190},
    {(void*)250, 200},
    {(void*)250, 210}
  };

/* Huvudprogrammet utvärderar din lösning. */
int main()
{
  int i;
  bool result;
  
  /* Testa algoritmen med ett givet intervall (en buffer). */
  /*for (i = 0; i < TEST_CASE_COUNT; ++i)
    {
      start_evaluate_algorithm(test_case[i].start, test_case[i].length);
      result = verify_fix_length(test_case[i].start, test_case[i].length);
      evaluate(result);
      end_evaluate_algorithm();
    }
  */
  /* Testa algoritmen med en sträng. */
  for (i = 0; i < TEST_CASE_COUNT; ++i)
    {
      start_evaluate_algorithm(test_case[i].start, test_case[i].length);
      result = verify_variable_length(test_case[i].start);
      evaluate(result);    
      end_evaluate_algorithm();
    }
  return 0;
}
