/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include <cstdlib>
#include <ctime>

int CharType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS, "invalid type");
  return common::compare_string(
      (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
}

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}




RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{

  switch (type) {
    case AttrType::DATES: {
      int i     = 0;
      int dates = 0;
      string yy,mm,dd;
      //convert char* to yymmdd
      while (val.value_.pointer_value_[i] >='0'&&val.value_.pointer_value_[i]<='9' && i < val.length_) {
        yy += val.value_.pointer_value_[i];
        i++;
      }
      i++;
      while (val.value_.pointer_value_[i] >='0'&&val.value_.pointer_value_[i]<='9' && i < val.length_) {
        mm += val.value_.pointer_value_[i];
        i++;
      }
      i++;
      while (val.value_.pointer_value_[i] >='0'&&val.value_.pointer_value_[i]<='9' && i < val.length_) {
        dd += val.value_.pointer_value_[i];
        i++;
      }
      if(mm.length()<2) {
        mm = '0'+mm;
      }
      if(dd.length()<2) {
        dd = '0'+dd;
      }
      struct tm tp;
      if(!strptime(val.value_.pointer_value_, "%Y-%m-%d", &tp)) {
        return RC::UNIMPLEMENTED;
      }
      dates = std::atoi((yy+mm+dd).c_str());
      // if (dates < 19700101 || dates > 20380200)
      //   return RC::UNIMPLEMENTED;
      result.set_int(dates);

      return RC::SUCCESS;
    }
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}