/**
 * @file   tdbpp_dense_read_subset_incomplete.cc
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2017 TileDB, Inc.
 * @copyright Copyright (c) 2016 MIT and Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * It shows how to read from a dense array, constraining the read
 * to a specific subarray and a subset of attributes. Moreover, the
 * program shows how to handle incomplete queries that did not complete
 * because the input buffers were not big enough to hold the entire
 * result.
 */

#include <tdbpp>
#include <iomanip>

int main() {
  using std::setw;
  tdb::Context ctx;

  // Init the array & query for the array
  tdb::Array array = ctx.array_get("my_dense_array");
  tdb::Query query = array.read();

  // Set subarray. Templated on domain type.
  query.subarray<tdb::type::UINT64>({3, 4, 2, 4});
  query.buffer_list({"a1"});
  query.layout(TILEDB_ROW_MAJOR);

  // Limit buff size to 2
  auto a1_data = query.make_buffer<tdb::type::INT32>("a1", 2);
  query.set_buffer<tdb::type::INT32>("a1", a1_data);

  std::cout << "a1\n---\n";
  do {
    std::cout << "Reading cells...\n";
    query.submit();
    const auto &buff_sizes = query.returned_buff_sizes();

    for (unsigned i = 0; i < buff_sizes[0]; ++i) {
      std::cout << a1_data[i] << "\n";
    }
  } while (query.query_status() == tdb::Query::Status::INCOMPLETE);

  return 0;
}