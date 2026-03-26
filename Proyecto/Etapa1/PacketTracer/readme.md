
# Configuración de Switches Cisco (S1 y S2)

## Descripción
En esta parte realizamos la configuración básica de dos switches (S1 y S2), incluyendo:

- Asignación de hostname
- Configuración de contraseñas
- Encriptación de contraseñas
- Acceso por consola
- Guardado de configuración
- Reinicio del dispositivo
- Verificación de la configuración

---

## Switch S1

### Configuración realizada

- Cambio de nombre del dispositivo:
```

hostname s1

```

- Configuración de contraseña privilegiada:
```

enable secret cisco

```

- Encriptación de contraseñas:
```

service password-encryption

```

- Configuración de consola:
```

line console 0
password netacad
login

```

---

### Guardado de configuración
```

copy running-config startup-config

```

---

### Reinicio
```

reload

```

---

### Verificación
- Solicitó contraseña de consola: `netacad`
- Acceso al modo privilegiado con: `cisco`
- La configuración se mantuvo después del reinicio

---



## Switch S2

### Configuración realizada

La misma del switch 1, con las misma contraseñas
Solo que el nombre del dispositivo es S2, es lo diferente.

---

## Conclusión

Se configuraron correctamente ambos switches con medidas básicas de seguridad y persistencia de configuración.  

Se comprendió el uso de los diferentes modos de IOS (`>`, `#`, `(config)#`) y se identificaron errores comunes en la escritura de comandos.

---

## Autor
Carlos Alvarado y Kiara Brenes



