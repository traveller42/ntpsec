def options_cmd(ctx, config):
	ctx.load("compiler_c")
	ctx.load("msvc")
	ctx.load('waf_unit_test')

	def callback_flags(option, opt, value, parser):
		config["OPT_STORE"].setdefault(opt, []).append(value)

	grp = ctx.add_option_group("NTP configure options")
	grp.add_option('--enable-debug', action='store_true', default=False, help="(ignored)")
	grp.add_option('--disable-debug', action='store_true', default=False, help="Disable debugging code")
	grp.add_option('--enable-debug-gdb', action='store_true', default=False, help="Enable GDB debugging symbols")
	grp.add_option('--enable-crypto', action='store_true', default=False, help="Enable OpenSSL.")
	grp.add_option('--disable-droproot', action='store_true', default=False, help="Disable dropping root.")
	grp.add_option('--disable-dns-lookup', action='store_true', default=False, help="Disable DNS lookups.")
	grp.add_option('--disable-dns-retry', action='store_true', default=False, help="Disable retrying DNS lookups.")
	grp.add_option('--disable-mdns-registration', action='store_true', default=False, help="Disable MDNS registration.")

	grp = ctx.add_option_group("NTP cross compile options")
	grp.add_option('--cross-compiler', type='string', help="Path to cross compiler CC. (enables cross-compiling)")
	grp.add_option('--cross-cflags', type='string',  action="callback", callback=callback_flags, help="Cross compiler CFLAGS.")
	grp.add_option('--cross-ldflags', type='string', action="callback", callback=callback_flags, help="Cross compiler LDFLAGS.")

	grp = ctx.add_option_group("NTP configure features")
	grp.add_option('--enable-leap-smear', action='store_true', default=False, help="Enable Leap Smearing.")
	grp.add_option('--enable-mssntp', action='store_true', default=False, help="Enable Samba MSS NTP support.")
	grp.add_option('--enable-lockclock', action='store_true', default=False, help="Enable NIST lockclock scheme.")

	grp = ctx.add_option_group("Refclock configure options")
	grp.add_option('--refclock', dest='refclocks', help="Comma-separated list of Refclock IDs to build (or \"all\")", type='string')
	grp.add_option('--list', action='store_true', default=False, help="List available Refclocks")

	grp = ctx.add_option_group("NTP developer configure options")
	grp.add_option('--enable-saveconfig', action='store_true', help="Enable configuration saving on exit.")
	grp.add_option('--build-version-tag', type='string', help="Append a tag to the version string.")
	grp.add_option('--cflags', type='string', action="callback", callback=callback_flags, help="Users should use CFLAGS in their environment.")
	grp.add_option('--ldflags', type='string', action="callback", callback=callback_flags, help="Users should use LDFLAGS in their environment.")
	grp.add_option('--enable-fortify', action='store_true', help="Enable HP Fortify.")
	grp.add_option('--fortify-flags', type='string', action='store', help="Fortify flags.")
	grp.add_option('--check', action='store_true', default=False, help="Run tests")
	grp.add_option('--enable-rtems-trace', action='store_true', default=False, help="Enable RTEMS Trace.")
	grp.add_option('--rtems-trace-path', type='string', default="", help="Path to rtems-tld.")

	grp = ctx.add_option_group("NTP documentation configure options")
	grp.add_option('--enable-doc', action='store_true', default=False, help="Build NTP documentation")
	grp.add_option('--enable-doc-only', action='store_true', default=False, help="Only build NTP documentation")
	grp.add_option('--enable-a2x-xmllint', action='store_true', default=False, help="Build NTP documentation with a2x XML lint")
	grp.add_option('--disable-manpage', action='store_true', default=False, help="Disable Manpage building.")
	grp.add_option('--path-doc', type='string', action='store', default=None, help="Force doc install directory.")

	grp = ctx.add_option_group("Not for general use")
	grp.add_option('--build-snapshot', action='store_true', default=False, help="Generate source snapshot.")
