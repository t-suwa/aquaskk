# -*- mode: ruby; coding: utf-8 -*-

class KeymapTranslator 
  # ------------------------------------------------------------
 private
  def initialize(path, user_defaults)
    @path = path
    @user_defaults = user_defaults
    @events = {
      'SKK_ENTER'      => [ 'group::hex::0x03,0x0a,0x0d', 'ctrl::m' ],
      'SKK_BACKSPACE'  => [ 'hex::0x08', 'ctrl::h' ],
      'SKK_DELETE'     => [ 'hex::0x7f', 'ctrl::d' ],
      'SKK_TAB'        => [ 'hex::0x09', 'ctrl::i' ],
      'SKK_LEFT'       => [ 'hex::0x1c', 'ctrl::b' ],
      'SKK_RIGHT'      => [ 'hex::0x1d', 'ctrl::f' ],
      'SKK_UP'         => [ 'hex::0x1e', 'ctrl::a' ],
      'SKK_DOWN'       => [ 'hex::0x1f', 'ctrl::e' ],
      'SKK_PING'       => [ 'ctrl::l' ],
      'SKK_UNDO'       => [ 'ctrl::/' ],
    }

    @attributes = {
      'UpperCases'     => [ 'group::A-K,M-P,R-Z' ],
      'Direct'         => [ 'group::keycode::0x41,0x43,0x45,0x4b,0x4e,0x51-0x59,0x5b,0x5c,0x5f' ],
      'PrevCompletion' => [ ',' ],
      'NextCompletion' => [ '.' ],
      'InputChars'     => [ 'group::hex::0x20-0x7e' ],
      'CompConversion' => [ 'alt::hex::0x20', 'shift::hex::0x20' ],
    }

    @handle_option = {
      'AlwaysHandled'  => [ 'group::keycode::0x66,0x68' ],
      'PseudoHandled'  => [ 'ctrl::l||hex::0x1b' ],
    }
  end

  def convert(value)
    value.sub(/\\"/, 'hex::0x22').gsub(/"/, '').sub(/\\C-/, 'ctrl::').sub(/\\/, '')
  end

  def update(hash, symbol, value)
    result = convert(value)

    if hash[symbol] then
      hash[symbol].push(result)
    else
      hash[symbol] = [ result ]
    end
  end

  def write(file, hash, comment)
    file.print <<EOF

# ============================================================
# #{comment}
# ============================================================

EOF

    hash.each_key { |key|
      file.printf("%-24s%s\n", key, hash[key].uniq.join('||'))
    }
  end

  # ------------------------------------------------------------
  public

  def execute
    File.new(@path).each_line { |line|
      next if line.empty? or line =~ /^#/

      symbol, value = line.sub(/" "/, 'hex::0x20').split

      hash = @events;

      case symbol
      when 'kana-mode-key'
        update(hash, 'SKK_JMODE', value)

      when 'cancel-key'
        update(hash, 'SKK_CANCEL', value);

      when 'paste-key'
        update(hash, 'SKK_PASTE', value);
      end

      hash = @attributes;

      case symbol
      when 'toggle-kana-key'
        update(hash, 'ToggleKana', value);

      when 'toggle-katakana-key'
        update(hash, 'ToggleJisx0201Kana', value);

      when 'latin-mode-key'
        update(hash, 'SwitchToAscii', value);

      when 'jisx0208-latin-mode-key'
        update(hash, 'SwitchToJisx0208Latin', value);

      when 'abbrev-mode-key'
        update(hash, 'EnterAbbrev', value);

      when 'prev-kouho-key'
        update(hash, 'PrevCandidate', value);

      when 'next-kouho-key'
        update(hash, 'NextCandidate', value);

      when 'purge-from-jisyo-key'
        update(hash, 'RemoveTrigger', value);

      when 'abbrev-prev-key'
        update(hash, 'PrevCompletion', value);

      when 'abbrev-next-key'
        update(hash, 'NextCompletion', value);

      when 'set-henkan-point-key'
        update(hash, 'EnterJapanese', value);

      when 'use-eisuu-to-set-henkan-point'
        if value == 'true' then
          update(hash, 'EnterJapanese', 'keycode::0x66')
        end

      when 'use-kana-to-set-henkan-point'
        if value == 'true' then
          update(hash, 'EnterJapanese', 'keycode::0x68')
        end
      end

      if symbol == 'fix-n' then
        @user_defaults.write('fix_intermediate_conversion', value == 'true' ? '1' : '0')
      end

      if symbol == 'candidate-labels' then
        @user_defaults.write('candidate_labels', value)
      end
    }
  end

  def save(path)
    File.open(path, File::CREAT|File::TRUNC|File::WRONLY) { |file|
      write(file, @events, 'event section')
      write(file, @attributes, 'attribute section (for SKK_CHAR)')
      write(file, @handle_option, 'handle option')
    }
  end
end



