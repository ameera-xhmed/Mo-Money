void History::sort_by_date(){
Transaction *now; //current node 
  Transaction *next; //next node 
  Transaction *New = nullptr; //new head 

  Transaction *temp;

  if(p_head == nullptr){
    return;
  }
 
  while(p_head != nullptr){ //checks that its not last in the list 
    temp = p_head; //temp list with phead values
    p_head = p_head->get_next(); //get next phead

    temp->set_next(nullptr);//set next phead in the temp to null

    if(New == nullptr){
      New = temp; //new head of list is temp
    }
    else{
      now = New; //current 
      next = New->get_next(); //next

      if(*temp < *New){
        temp->set_next(New);
        New = temp;
        continue; //restart the loop
      }

      while((next != nullptr) && (*next < *temp)){
        now = next;
        next = next->get_next();
      }

      if(next == nullptr){
        now->set_next(temp);
      }
      else{
        now->set_next(temp);
        temp->set_next(next);
      }
    }
  }
  p_head = New;
  }