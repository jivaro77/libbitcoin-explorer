/**
 * Copyright (c) 2011-2014 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin-explorer.
 *
 * libbitcoin-explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <bitcoin/explorer/utility/parameter.hpp>

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <bitcoin/explorer/define.hpp>
#include <bitcoin/explorer/utility/utility.hpp>

using namespace bc::explorer;

const int parameter::not_positional = -1;
const char parameter::no_short_name = 0x00;
const char parameter::option_prefix_char = '-';

// 100% component coverage, common scenarios.
// A required argument may only be preceeded by required arguments.
// Requiredness may be in error if the metadata is inconsistent.
void parameter::initialize(const option_metadata& option,
    const argument_list& arguments)
{
    set_position(position(option, arguments));
    set_args_limit(arguments_limit(get_position(), option, arguments));
    set_required(option.semantic()->is_required());
    set_long_name(option.long_name());
    set_short_name(short_name(option));
    set_description(option.description());
    set_format_name(option.format_name());
    set_format_parameter(option.format_parameter());
}

// 100% component coverage, all three scenarios (long, short, both)
int parameter::position(const option_metadata& option,
    const argument_list& arguments) const
{
    return find_pair_position(arguments, option.long_name());
}

// 100% unit coverage, all three scenarios (long, short, both)
char parameter::short_name(const option_metadata& option) const
{
    // This call requires boost 1.50, don't use it.
    //auto name = option.canonical_display_name(
    //    search_options::dashed_short_prefer_short);

    // This is a substitute that allows us to use boost 1.49 for libbitcoin.
    auto name = split(option.format_name()).front();
    auto is_short_name = name[0] == option_prefix_char && 
        name[1] != option_prefix_char;

    return if_else(is_short_name, name[1], no_short_name);
}

// 100% component coverage
unsigned parameter::arguments_limit(int position, 
    const option_metadata& option, const argument_list& arguments) const
{
    if (position == parameter::not_positional)
        return option.semantic()->max_tokens();

    return arguments[position].second;
}