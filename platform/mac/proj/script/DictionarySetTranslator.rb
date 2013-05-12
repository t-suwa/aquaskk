# -*- mode: ruby; coding: utf-8 -*-

require 'rexml/document'

class DictionarySetTranslator
  def initialize(path)
    File.open(path) { |file|
      @plist = REXML::Document.new(file)
    }
  end

  def execute
    root = @plist.root

    root.elements.each('array/dict') { |dict|
      elem = dict.elements["integer"]
      type = elem.text.to_i

      case type
      when 10, 11
        elem.text = type - 10

      when 20
        elem.text = '3'

      when 30
        elem.text = '2'

      else
        root.delete_element(dict.xpath)
      end
    }
  end

  def save(path)
    File.open(path, File::CREAT|File::TRUNC|File::WRONLY) { |file|
      @plist.write file
    }
  end
end
