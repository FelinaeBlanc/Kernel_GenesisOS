int atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Si le nombre est négatif, gérer le signe
    if (str[0] == '-') {
        sign = -1;
        i++;
    }

    // Parcourir tous les caractères du nombre
    for (; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            // Si ce n'est pas un chiffre, retourner 0
            return 0;
        }
        result = result * 10 + str[i] - '0';
    }

    // Retourner le nombre avec le signe approprié
    return sign * result;
}