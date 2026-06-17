# ELDC

Ruby binding for [Efficient Language Detector (written in C) (ELDC)](https://github.com/nitotm/eldc).

Project repository: https://github.com/gemmaro/ruby-eldc

## Installation

Install the gem and add to the application's `Gemfile` by executing:

```bash
bundle add eldc
```

If bundler is not being used to manage dependencies, install the gem by executing:

```bash
gem install eldc
```

## Usage

```ruby
detector = ELDC::Detector.instance
detector.detect("Bonjour le monde") #=> "fr"

detector.detect_details("Bonjour le monde")
#=> #<ELDC::DetectResult
#     lang=fr
#     reliable=1
#     scores=[#<ELDC::ScoreItem
#               lang=fr
#               score=0.9016362428665161>,
#             #<ELDC::ScoreItem
#               lang=de
#               score=0.5073475241661072>,
#             #<ELDC::ScoreItem
#               lang=no
#               score=0.5018803477287292>]>
```

See more details in the API documentation.

## Development

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and the created tag, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## License

```text
Copyright 2026 gemmaro

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
