#include<stdio.h> 
#include<string.h>

#define CGST_RATE 0.025
#define SGST_RATE 0.025
#define Discount_1 0.1
#define Discount_2 0.15
#define Discount_3 0.2

struct entry {   
  char Item_Name[50];
  float Price;
  float quantity;
}; 

void Add_Product(struct entry *e) { 
  char choice;
  FILE *fptr;
  do{
  printf("\n     Enter the product details     \n");
  printf("Enter product name: ");
  getchar();
  fgets(e->Item_Name, sizeof(e->Item_Name), stdin);
  e->Item_Name[strcspn(e->Item_Name, "\n")] = '\0';

  for(int i = 0; e->Item_Name[i]; i++)
  { if(e->Item_Name[i] == ' ')
    e->Item_Name[i] = '_'; }

  fptr = fopen("Product.txt", "r");
  if (fptr == NULL)
  { printf("Error. File doesn't Exist.");
    return; }

  char item[50];
  float itemprice, itemquantity;
  int found = 0;

  while(fscanf(fptr, "%s | %f | %f\n", item, &itemprice, &itemquantity) == 3)
  { if(strcasecmp(item, e->Item_Name) == 0)
  { found = 1;
    break; }
  }
  fclose(fptr);

  if(found)
  { printf("%s is already saved. Please enter another item.\n", item); }
  else
  { fptr = fopen("Product.txt", "a");
    printf("Enter price(per unit/kg/litre): ");
    scanf("%f", &e->Price);
    printf("Enter initial quantity: ");
    scanf("%f", &e->quantity);
    fprintf(fptr, "%s | %.2f | %.2f\n", e->Item_Name, e->Price, e->quantity);
    fclose(fptr);
  }

  printf("\nWant to add another? (y/n) ");
  scanf(" %c", &choice);
  } while(choice == 'y' || choice == 'Y');
}

void Delete_Entry(struct entry *e){
  char target[50], choice;
  do{
  FILE *fptr, *fp;
  fptr = fopen("Product.txt", "r");
  fp = fopen("text.txt", "w");

  if(fp == NULL || fptr == NULL)
  { printf("Error. File doesn't exist\n.");
    return ; }
  printf("\nEnter the item name: ");
  getchar();
  fgets(target, sizeof(target), stdin);
  target[strcspn(target, "\n")] = '\0';

  for(int i = 0; target[i]; i++)
  {if(target[i] == ' ')
  { target[i] = '_'; }
  }

  while(fscanf(fptr, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
  { if(strcasecmp(target, e->Item_Name) == 0)
  { continue; }
  else
  { fprintf(fp, "%s | %.2f | %.2f\n", e->Item_Name, e->Price, e->quantity); }
  }

  fclose(fp);
  fclose(fptr);
  remove("Product.txt");
  rename("text.txt", "Product.txt");
  printf("\nWant to delete another? (y/n) ");
  scanf(" %c", &choice);

  } while(choice == 'y' || choice == 'Y');

  printf("\nItem(s) deleted succesfully!");
}

void Bill(struct entry *e){
  char item[50], choice, list[500][50];
  int i = 0;
  float quantity[500], cost[500], temp;
  FILE *fptr;
  fptr = fopen("Product.txt", "r");
  if(!fptr)
  { printf("Error. File not found.\n");
    return; }
  printf("-------MENU-------\n");
  printf("Item         Price\n");
  while(fscanf(fptr, "%s | %f | %f\n", e->Item_Name, &e->Price, &e->quantity) == 3)
  { printf("%s      %.2f\n", e->Item_Name, e->Price); }
  printf("------------------\n");
  
  do{
  int found = 0;
  printf("\nEnter an item: ");
  getchar();
  fgets(item, sizeof(item), stdin);
  item[strcspn(item, "\n")] = '\0';
  for(int j = 0; item[j]; j++)
  { if(item[j] == ' ')
    item[j] = '_';
  }
  printf("Enter quantity(in Kg/ Litre/ unit): ");
  scanf("%f", &temp);
  rewind(fptr);
  while(fscanf(fptr, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
  { if(strcasecmp(item, e->Item_Name) == 0)
  { if(temp > e->quantity)
  { printf("Not enough stock. Available stock: %.2f\n", e->quantity);
    found = 1; }
  else
  { strcpy(list[i], item);
    cost[i] = e->Price;
    found = 1;
    quantity[i] = temp;
    i++;
    fclose(fptr);
    FILE *readf = fopen("Product.txt", "r");
    FILE *tempf = fopen("temp.txt", "w");
    if(!tempf)
    { printf("File not found");
      return; }
    rewind(readf);
    while(fscanf(readf, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
    {if(strcasecmp(item, e->Item_Name) == 0)
    { e->quantity -= temp; }
    fprintf(tempf, "%s | %f | %f\n", e->Item_Name, e->Price, e->quantity);
    }
  fclose(readf);
  fclose(tempf);
  remove("Product.txt");
  rename("temp.txt", "Product.txt");
  fptr = fopen("Product.txt", "r");
  }
  break;
  }
  }
  if(!found)
  printf("Item not found\n");
  printf("Want to take another (y/n): ");
  scanf(" %c", &choice);
  } while(choice == 'y' || choice == 'Y');
  fclose(fptr);

  float s, total = 0;
  printf("\n                 BILL           %s\n", __TIME__);
  printf("========================================\n");
  printf("ITEM      QUANTITY      RATE      AMOUNT\n");
  printf("========================================\n");

  for(int j=0; j<i; j++)
  { s = quantity[j] * cost[j];
    printf("\n%s     %.2f       %.2f      %.2f", list[j], quantity[j], cost[j], s);
    total +=  s; }
  float CGST = CGST_RATE * total;
  float SGST = SGST_RATE * total;

  float discount = 0;
  if(total >= 500 && total <= 1000)
  { discount = Discount_1 * total; }
  else if(total >= 1000 && total <= 1500)
  { discount = Discount_2 * total; }
  else if(total >= 1500)
  { discount = Discount_3 * total; }

  printf("\nTOTAL                          Rs %.2f\n",total);
  printf("----------------------------------------\n");
  printf("+CGST                 2.50%%       %.2f\n", CGST);
  printf("+SGST                 2.50%%       %.2f\n", SGST);
  if(discount != 0)
  { printf("Discount                       Rs %.2f\n", discount); }
  printf("----------------------------------------\n");
  float net;
  net = total + CGST + SGST - discount;
  printf("NET PAYABLE                      Rs %.2f\n", net); 
  printf("========================================\n");
  printf("E & O. E.         Thanks... Visit Again...\n");
}

void Inventory(struct entry *e){
  float assest = 0;
  FILE *readf = fopen("Product.txt", "r");
  if(!readf)
  { printf("Error. File not found!\n");
    return  ; }
  printf("------------CURRENT INVENTORY------------\n");
  printf("ITEM      RATE      QUANTITY      STATUS\n");
  while(fscanf(readf, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
  { assest = assest + (e->Price * e->quantity);
    if(e->quantity >= 10)
    { printf("%s   %.2f   %.2f   Okay\n", e->Item_Name, e->Price, e->quantity); }
    else if(e->quantity < 10 && e->quantity > 0)
    { printf("%s   %.2f   %.2f   Low\n", e->Item_Name, e->Price, e->quantity); }
    else
    { printf("%s   %.2f   %.2f   Out of Stock\n", e->Item_Name, e->Price, e->quantity); }
  }
  printf("-------------------------------\n");
  printf("Total cost of inventory: %.2f", assest);
  rewind(readf);
  while(fscanf(readf, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
  { if(e->quantity <= 5)
  { printf("Please add the following units as they are low on stock: ");
    printf("%s\n", e->Item_Name); }
  }
  fclose(readf);
}

void Update_Inventory(struct entry *e){
  char choice;
  
  do{ 
  char item[50];
  float new_quantity;
  int found = 0;
  FILE *fptr = fopen("Product.txt", "r");
  if(!fptr)
  { printf("File not found!\n");
    return; }
  printf("Enter item name: ");
  getchar();
  fgets(item, sizeof(item), stdin);
  item[strcspn(item, "\n")] = '\0';
  while(fscanf(fptr, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
  { if(strcasecmp(item, e->Item_Name) == 0)
  { printf("Enter the quantity to order: ");
    scanf("%f", &new_quantity);
    fclose(fptr);

    FILE *fp;
    fp = fopen("Product.txt", "r");
    FILE *tempf = fopen("temp.txt", "w");
    while(fscanf(fp, "%s | %f | %f", e->Item_Name, &e->Price, &e->quantity) == 3)
    { if(strcasecmp(item, e->Item_Name) == 0)
    { e->quantity += new_quantity; }
    fprintf(tempf, "%s | %.2f | %.2f\n", e->Item_Name, e->Price, e->quantity); 
    }
  found = 1;
  fclose(fp);
  fclose(tempf);
  remove("Product.txt");
  rename("temp.txt", "Product.txt");
  break;
  }
  if(found != 1)
  { printf("Item not found\n");
    printf("Please add the product by operation 1.\n"); }
  }
  printf("Would you like to order more? (y/n) ");
  scanf(" %c", &choice);
  fclose(fptr);
  } while(choice == 'y' || choice == 'Y');
}

int main()
{ 
  struct entry e;
  char choice;
  
  printf("Greetings, User!\n");
  printf("How may I assist you?\n\n");
  
  FILE *fp = fopen("Product.txt", "r");
  if(!fp)
  { printf("File not found!\n");
    return; }
  while(fscanf(fp, "%s | %f | %f", e.Item_Name, &e.Price, &e.quantity) == 3)
  { if(e.quantity < 10)
  { printf("!! Low Stock Alert !!\n");;
    printf("%s: Stock reached %.2f.\n", e.Item_Name, e.quantity); 
  }
  }
  fclose(fp);
  do{
  int operation;
  struct entry E;
  
  printf("Add a new product (1)\n");
  printf("Delete a product (2)\n");
  printf("Make a Bill (3)\n");
  printf("Check Inventory(4)\n");
  printf("Update Inventory(5)\n");
  printf("Exit (6)\n");
  printf("\nChoose a operation: ");
  scanf("%d", &operation);

  switch (operation)
  {
  case 1:
    Add_Product(&e);
    break;

  case 2:
    Delete_Entry(&e);
    break;

  case 3:
    Bill(&e);
    break;

  case 4:
    Inventory(&e);
    break;

  case 5:
    Update_Inventory(&e);
    break;
  
  case 6:
    printf("Thank You for you using BillPro!\n");
    printf("Exiting...");
    return 0;
  
  default:
    printf("Error.\n");
    printf("Operation does not exist.");
    break;
  }
  printf("\nAnother Operation? (y/n): ");
  scanf(" %c", &choice);
  printf("\n");
  if(choice == 'n' || choice == 'N')
  { printf("Thank You for you using BillPro!\n");
    printf("Exiting...");
    break;
  }
  } while(choice == 'y' || choice == 'Y');
return 0;
}