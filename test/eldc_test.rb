# frozen_string_literal: true

# Copyright 2026 gemmaro
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require "test_helper"

class ELDCTest < Test::Unit::TestCase
  def setup
    @detector = ELDC::Detector.instance
  end

  test "VERSION" do
    assert do
      ::ELDC.const_defined?(:VERSION)
    end
  end

  test "detect" do
    assert_equal "fr", @detector.detect("Bonjour le monde")
    assert_nil @detector.detect("12345 !@#")
  end

  test "max scores" do
    assert_instance_of Integer, ELDC::MAX_SCORES
  end

  test "detect details" do
    result = @detector.detect_details("Bonjour le monde")
    assert_instance_of ELDC::DetectResult, result
    assert_equal "fr", result.language
    assert_true result.reliable
    assert_instance_of String, result.scores[0].language
    assert_instance_of Float, result.scores[0].score
  end

  test "set languages" do
    assert_equal ["en","fr","es"], @detector.set_languages("en,fr,es")
    assert_equal ["en","fr"], @detector.set_languages("eng,fra,xyz")
    assert_equal "fr", @detector.detect("Bonjour le monde")
    @detector.languages = ""
    assert_equal "es", @detector.detect("Hola mundo")

    @detector.languages = ["en", "es"]
    assert_not_equal "fr", @detector.detect("Bonjour le monde")
    @detector.languages = ""
    assert_equal "fr", @detector.detect("Bonjour le monde")
  end

  test "set scheme" do
    @detector.scheme = "iso639-2t"
    assert_equal "fra", @detector.detect("Bonjour")
    @detector.scheme = "iso639-1"
    assert_equal "fr", @detector.detect("Bonjour")
  end

  test "set scores" do
    @detector.scores = 2
    assert_equal 2, @detector.detect_details("Was ist das?").scores.size
  end
end
