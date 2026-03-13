find_package(PkgConfig OPTIONAL)

if(PkgConfig_FOUND)
    pkg_check_modules(GMPXX REQUIRED gmpxx)
else()
    message(ERROR "You don't have PkgConfig, feel free to implement GMP find logic however you like.")
endif()
