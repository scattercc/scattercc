module Overcommit::Hook::PreCommit
  class CppHeaderIncludeOnceGuard < Base
    def run
      messages = []

      applicable_files.map do |file_name|
        file = File.open(file_name)
        messages += check_file(file, file_name)
      end

      messages
    end

    private

    def check_file(file, file_name)
      messages_for_file = []

      if not file_name.start_with?(Overcommit::Utils.repo_root)
        messages_for_file << Overcommit::Hook::Message.new(
          :error,
          file_name,
          1,
          "File #{file_name} path doesn't start with #{Overcommit::Utils.repo_root} ??"
        )
        return
      end

      first_line = ""
      second_line = ""
      last_line = ""
      last_line_number = ""

      file.each_line do |line|
        first_line = line.rstrip if file.lineno == 1
        second_line = line.rstrip if file.lineno == 2
        last_line = line.rstrip
        last_line_number = file.lineno
      end

      rel_name = file_name[(Overcommit::Utils.repo_root.length + 1)..-1]
      guard_macro = "_" + rel_name.split(/[^A-Za-z0-9]/).join("_").upcase + "_INCLUDED_"

      no_guard_first_line = "// ReSharper disable once CppMissingIncludeGuard"
      no_guard_second_line = "// No include-once guard: THIS IS INTENTIONAL!"

      guard_first_line = "#if !defined(#{guard_macro})"
      guard_second_line = "#define #{guard_macro}"
      guard_last_line = "#endif  // !defined(#{guard_macro})"

      if first_line == no_guard_first_line
        if second_line != no_guard_second_line
          messages_for_file << Overcommit::Hook::Message.new(
            :error,
            file_name,
            2,
            "#{file_name}: Expect intentional tip on second line: \"#{no_guard_second_line}\""
          )
        end

      elsif first_line.start_with?("#if")
        if first_line != guard_first_line
          messages_for_file << Overcommit::Hook::Message.new(
            :error,
            file_name,
            1,
            "#{file_name}: Expect first line: \"#{guard_first_line}\", but got: \"#{first_line}\""
          )
        end
        if second_line != guard_second_line
          messages_for_file << Overcommit::Hook::Message.new(
            :error,
            file_name,
            2,
            "#{file_name}: Expect second line: \"#{guard_second_line}\", but got: \"#{second_line}\""
          )
        end
        if last_line != guard_last_line
          messages_for_file << Overcommit::Hook::Message.new(
            :error,
            file_name,
            last_line_number,
            "#{file_name}: Expect last line: \"#{guard_last_line}\", but got: \"#{last_line}\""
          )
        end

      else
        messages_for_file << Overcommit::Hook::Message.new(
          :error,
          file_name,
          1,
          "#{file_name}: Unknown first line: Expect either \"#{guard_first_line}\" or \"#{no_guard_first_line}\""
        )
      end

      messages_for_file
    end
  end
end
