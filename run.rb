#!/usr/bin/ruby
#
# (c) Copyright M. Shahab SANJARI 2011
#
# this is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# barion is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
#
# along with this software.  If not, see <http://www.gnu.org/licenses/>.


begin
  raise 'Please specify the iqt filename.' if not ARGV.size == 1
  file=ARGV[0].to_s
  puts file
  system('. rootlogin')
  system('make')
  system("./iqt2time -i #{file} -o data")
  system('./spectrumgen -w 1024 -n 1000 -i data.time.bin -o data')
  system('./spectrogramme -w 1024 -n 1000 -i data.psd.bin')
rescue Exception => err
  puts "#{err}"
end
