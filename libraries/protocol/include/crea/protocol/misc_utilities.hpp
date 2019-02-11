#pragma once

namespace crea { namespace protocol {

enum curve_id
{
   quadratic,
   bounded_curation,
   linear,
   square_root
};

} } // crea::utilities


FC_REFLECT_ENUM(
   crea::protocol::curve_id,
   (quadratic)
   (bounded_curation)
   (linear)
   (square_root)
)
