# frozen_string_literal: true

require_relative "lib/eldc/version"

Gem::Specification.new do |spec|
  spec.name = "eldc"
  spec.version = ELDC::VERSION
  spec.authors = ["gemmaro"]
  spec.email = ["gemmaro.dev@gmail.com"]

  spec.summary = "Language detector library"
  spec.description = "This is a Ruby binding library for Efficient Language Detector (Written in C) (ELDC)."
  spec.license = "Apache-2.0"
  spec.homepage = "https://github.com/gemmaro/ruby-eldc"
  spec.required_ruby_version = ">= 3.2.0"
  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/gemmaro/ruby-eldc"
  spec.metadata["changelog_uri"] = "https://github.com/gemmaro/ruby-eldc/blob/main/CHANGELOG.md"

  spec.files = Dir[
    'ext/eldc/eldc/src/eldc/eldc_lib.h',
    'ext/eldc/eldc/src/eldc/eldc_lib.c',
    'ext/eldc/eldc/src/eldc/eld_core.c',
    'ext/eldc/eldc/src/eldc/large_db.h',
    'ext/eldc/eldc/src/eldc/eld_unicode_bits.c',
    'ext/eldc/eldc/src/eldc/eld_iso639_2t.h',
    'ext/eldc/eldc/src/eldc/eld_tolower.h',
    'ext/eldc/eldc/LICENSE',
    'ext/eldc/extconf.rb',
    'ext/eldc/rb_eldc.c',
    'ext/eldc/rb_eldc.h',
    'lib/**/*.rb',
    'LICENSE',
    '*.md',
  ]
  spec.require_paths = ["lib"]
  spec.extensions = ["ext/eldc/extconf.rb"]
end
