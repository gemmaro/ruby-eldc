# frozen_string_literal: true

#--
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
#++

require_relative "eldc/version"
require "singleton"
require "eldc/eldc"

# The main entrypoint should be ELDC::Detector#instance.
module ELDC
  class Detector
    include Singleton

    alias languages= set_languages

    ##
    # :method: instance
    # :call-seq:
    #   instance -> ELDC::Detector
  end

  class DetectResult
    def inspect
      "#<#{self.class.name} lang=#{language} reliable=#{reliable} scores=#{scores}>"
    end
  end

  class ScoreItem
    def inspect
      "#<#{self.class.name} lang=#{language} score=#{score}>"
    end
  end
end
