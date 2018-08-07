#pragma once

namespace fc {
class uint128;
class variant;
} // fc

namespace creativecoin { namespace protocol {
template< typename Storage = fc::uint128 >
class fixed_string_impl;

class asset_symbol_type;
class legacy_creativecoin_asset_symbol_type;
struct legacy_creativecoin_asset;
} } // creativecoin::protocol

namespace fc { namespace raw {

template<typename Stream>
inline void pack( Stream& s, const uint128& u );
template<typename Stream>
inline void unpack( Stream& s, uint128& u );

template< typename Stream, typename Storage >
inline void pack( Stream& s, const creativecoin::protocol::fixed_string_impl< Storage >& u );
template< typename Stream, typename Storage >
inline void unpack( Stream& s, creativecoin::protocol::fixed_string_impl< Storage >& u );

template< typename Stream >
inline void pack( Stream& s, const creativecoin::protocol::asset_symbol_type& sym );
template< typename Stream >
inline void unpack( Stream& s, creativecoin::protocol::asset_symbol_type& sym );

template< typename Stream >
inline void pack( Stream& s, const creativecoin::protocol::legacy_creativecoin_asset_symbol_type& sym );
template< typename Stream >
inline void unpack( Stream& s, creativecoin::protocol::legacy_creativecoin_asset_symbol_type& sym );

} // raw

template< typename Storage >
inline void to_variant( const creativecoin::protocol::fixed_string_impl< Storage >& s, fc::variant& v );
template< typename Storage >
inline void from_variant( const variant& v, creativecoin::protocol::fixed_string_impl< Storage >& s );

inline void to_variant( const creativecoin::protocol::asset_symbol_type& sym, fc::variant& v );

inline void from_variant( const fc::variant& v, creativecoin::protocol::legacy_creativecoin_asset& leg );
inline void to_variant( const creativecoin::protocol::legacy_creativecoin_asset& leg, fc::variant& v );

} // fc
