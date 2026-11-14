# main/: app_main e o bootstrap — o único lugar que conhece as classes
# concretas e monta tudo.
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES app services motor battery led platform
)
