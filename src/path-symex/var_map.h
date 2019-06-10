/*******************************************************************\

Module: Variable Numbering

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

/// \file
/// Variable Numbering

#ifndef CPROVER_PATH_SYMEX_VAR_MAP_H
#define CPROVER_PATH_SYMEX_VAR_MAP_H

#include <iosfwd>
#include <map>

#include <util/namespace.h>
#include <util/type.h>
#include <util/std_expr.h>
#include <util/symbol_table.h>

extern irep_idt ID_C_full_identifier;

class var_mapt
{
public:
  explicit var_mapt(const namespacet &_ns);

  struct var_infot
  {
    enum { SHARED, THREAD_LOCAL, PROCEDURE_LOCAL } kind;

    bool is_shared() const
    {
      return kind==SHARED;
    }

    // the variables are numbered
    unsigned number;

    // full_identifier=symbol+suffix
    irep_idt full_identifier, symbol, suffix;

    // the symbol-member-index expression
    exprt original;

    unsigned ssa_counter;

    var_infot():kind(SHARED), number(0), ssa_counter(0)
    {
    }

    irep_idt ssa_identifier() const;
    symbol_exprt ssa_symbol() const;

    void increment_ssa_counter()
    {
      ++ssa_counter;
    }

    void output(std::ostream &out) const;
  };

  typedef std::map<irep_idt, var_infot> id_mapt;
  id_mapt id_map;

  var_infot &operator()(
    const irep_idt &symbol,
    const irep_idt &suffix,
    const exprt &original);

  var_infot &operator()(const symbol_exprt &original);

  var_infot &operator[](const irep_idt &full_identifier)
  {
    return id_map[full_identifier];
  }

  void clear()
  {
    shared_count=0;
    local_count=0;
    nondet_count=0;
    dynamic_count=0;
    id_map.clear();
  }

  void init(var_infot &var_info);

  const namespacet ns;
  symbol_tablet new_symbols;

  void output(std::ostream &) const;

protected:
  unsigned shared_count, local_count;

public:
  unsigned nondet_count;  // free inputs
  unsigned dynamic_count; // memory allocation

  static bool is_unbounded_array(const array_typet &);
  static bool is_unbounded_array(const typet &);
};

#endif // CPROVER_PATH_SYMEX_VAR_MAP_H
